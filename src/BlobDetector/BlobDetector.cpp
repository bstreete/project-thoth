#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

using namespace cv;

#define DEBUG 1     // Set to 0 for release

// cmake . && make && ./BlobDetector -i ../../input/st_2472776818_vAUTOLABEL.ppm -o test.png

// Change from contours? 
  // 
// Global debug flag
int debug = 0; 

// Main component prototypes
Mat smooth_input(Mat img, int count, int size, int type);
std::vector<std::vector<Point>> find_contours(Mat img, Mat gray);
Mat combine_clusters(Mat img, int range); 
Point linkBlobLeft(int range, Mat& input, KeyPoint blob);
Mat imbinary(Mat im);
Mat iminvert(Mat im);
double calc_area(std::vector<std::vector<Point>> contours);

// Miscellaneous helper prototypes
int distance(int x1, int y1, int x2, int y2); 
int check_args(int *size, int *type, int *count, int *range, std::string *file_name, 
  std::string *output_name, int argc, char** argv); 
void usage(char* binary);

/** @function main */
int main( int argc, char** argv ){
  std::string file_name, output_name;
  int size = 5, type = MORPH_ELLIPSE, count = 5, status, range = 10;
  Mat src, gray_scale, output;
  std::vector<std::vector<Point>> contours;
  double area; 

  // Check the passed arguments
  status = check_args(&size, &type, &count, &range, &file_name, &output_name, argc, argv);
  // Usage or help requested
  if (status == -1) return 0;
  // Invalid arguments passed
  if (status == 1) return 1;

  // Read the image
  src = imread(file_name);

  // Ensure the image loaded
  if( !src.data){ fprintf(stderr, "Unable to load %s. Verify the path and try again.\n", argv[1]); return 1; }

  // Threshold the image - BGR
  inRange(src, Scalar(0,0,150), Scalar(255,255,255), gray_scale);

  // Erode the image
  src = smooth_input(gray_scale, count, size, type);

  // Automagically cluster nearby danger zones
  src = combine_clusters(src, range);

  // Find contours of the clustered image
  contours = find_contours(src, gray_scale);

  // Calculate area
  area = calc_area(contours); 

  printf("Total Usable Area: %.02f%%\n", area * 100/ (src.rows * src.cols));

  // Draw the contours on the input image
  cvtColor(gray_scale, output, CV_GRAY2RGB);

  RNG rng(12345);
  for (int x = 0; x < contours.size(); x++) {
    Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
    drawContours( output, contours, x, color, -1, 8);
  }

  // Display the results if debug is enabled
  if (debug&&DEBUG) { imshow( "Result", output); waitKey(0); }

  // Save the eroded image for further comparisons
  imwrite(output_name, output);
  return 0;
}

/*
  Erodes and dilates the passed image multiple times determined by the value
  passed in count. The size determines the size of the pixel used
  to morph the original image. Type determines the type of pixel used.
  It expects MORPH_RECT, MORPH_CROSS, or MORPH_ELLIPSE for the type.
*/
Mat smooth_input(Mat src, int count, int size, int type){

  Mat img; 

  // Set the morph values for erosion
  Mat morph_values = getStructuringElement(type, Size(size, size));
  img = src.clone();

  // Morph the image 
  for (int x = 0; x < count; x++){
    erode(img, img, morph_values); 
    dilate(img, img, morph_values); 
  } 
  
  if (debug&&DEBUG) { imshow("After Erosion", img); waitKey(0); }

  return img;
}

/*
  Reads in an eroded image (img) and the thresholded original image(gray). 
  Finds the contours in the eroded image then draws the contours over the 
  thresholded image converted back to RGB. 
*/
std::vector<std::vector<Point>> find_contours(Mat img, Mat gray) {

  std::vector<std::vector<Point>> contours;
  std::vector<Vec4i> hierarchy; 
    // std::vector<Vec4i> lines; 
  Mat output, morph_values;

  // Find edges with canny 
  Canny (img, img, 50, 300, 5);

  // Display the results if debug is enabled
  if (debug&&DEBUG) { imshow( "Canny Result", img); waitKey(0); }

  morph_values = getStructuringElement(MORPH_CROSS, Size(3,3));
  
  dilate(img,img, morph_values);
  erode(img, img, morph_values);

  if (debug&&DEBUG) { imshow( "Canny After Morphing", img); waitKey(0); }

  // Find contours; Use external contours to prevent calculating area with internal and external
  findContours(img, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, Point(0,0));

  return contours;
}

/*
  Finds blobs of dangerous terrain and draws lines connecting blobs
  near to each other to segment the image. This simplifies the contour
  analysis which should also simplify area calculation. 
*/
Mat combine_clusters(Mat img, int range){

  img = imbinary(img);
    
    Mat output = img.clone();

    /*A blob detector to detect hazard zones; can be any size*/
    SimpleBlobDetector::Params hzrd_params;
    hzrd_params.filterByArea = true;
    hzrd_params.minArea = 0;
    hzrd_params.maxArea = 100000;
    hzrd_params.filterByConvexity = true;
    hzrd_params.minConvexity = 0;
    
    /*Not compatible below OpenCV Version 3.0*/
    Ptr<SimpleBlobDetector> hzrd_detect = SimpleBlobDetector::create(hzrd_params);
    
    std::vector<KeyPoint> hzrd_keypoints;
    hzrd_detect->detect(img, hzrd_keypoints);
    
    for (KeyPoint p1 : hzrd_keypoints) {
        int x1 = (int) p1.pt.x;
        int y1 = (int) p1.pt.y;
        
        bool perimeterLinked = false;
        
        /*Links together clusters of danger zones*/
        for (KeyPoint p2 : hzrd_keypoints) {
            int x2 = (int) p2.pt.x;
            int y2 = (int) p2.pt.y;
            int dist = distance(x1,y1,x2,y2);
            if (dist > 0 && dist < range)
            line(output, Point(x1,y1), Point(x2,y2), Scalar(0,0,0), 3);
        }
        /*Links blobs to above hazard zones within range*/
        for (int i = y1+1; i < img.rows; i++) {
            if (img.at<uchar>(i,x1) == 255) {
                for (int j = i+1; j < std::min(i+range,img.rows); j++) {
                    if (img.at<uchar>(j,x1) == 0) {
                        line(output, Point(x1,i-4), Point(x1,j+4), Scalar(0,0,0), 3);
                        perimeterLinked = true;
                        break;
                    }
                }
                break;
            }
        }
        
        /*Links blobs to below hazard zones within range*/
        for (int i = y1-1; i > 0; i--) {
            if (img.at<uchar>(i,x1) == 255) {
                for (int j = i-1; j > std::max(i-range,0); j--) {
                    if (img.at<uchar>(j,x1) == 0) {
                        line(output, Point(x1,i+4), Point(x1,j-4), Scalar(0,0,0), 3);
                        perimeterLinked = true;
                        break;
                    }
                }
                break;
            }
        }
        
        /*Links blobs to right hazard zones within range*/
        for (int i = x1+1; i < img.cols; i++) {
            if (img.at<uchar>(y1,i) == 255) {
                for (int j = i+1; j < std::min(i+range, img.cols); j++) {
                    if (img.at<uchar>(y1,j) == 0) {
                        line(output, Point(i-4, y1), Point(j+4,y1), Scalar(0,0,0), 3);
                        perimeterLinked = true;
                        break;
                    }
                }
                break;
            }
        }

        Point p2 = linkBlobLeft(range, img, p1);
        if ( !(p2.x == -1 && p2.y == -1) )
            line(output, Point(x1,y1), Point(p2.x-4, p2.y), Scalar(0,0,0), 3);
    }

  if (debug&&DEBUG) { imshow( "After Blob Clustering", output); waitKey(0); }

    return output; 
}

Point linkBlobLeft(int range, Mat& input, KeyPoint blob) {
    int x1 = (int) blob.pt.x;
    int y1 = (int) blob.pt.y;
    
    for (int i = x1-1; i > 0; i--) {
        if (input.at<uchar>(y1,i) == 255) {
            for (int j = i-1; j > std::max(i-range,0); j--) {
                if (input.at<uchar>(y1,j) == 0) {
                    return Point(j, y1);
                }
            }
            return Point(-1,-1);
        }
    }
    return Point(-1, -1);
}
/*Converts gray-scale image to binary image*/
Mat imbinary(Mat im) {
    Mat bnry = im.clone();
    for (int x = 0; x < bnry.rows; x++) {
        for (int y = 0; y < bnry.cols; y++) {
            uchar* pixel = &bnry.at<uchar>(x,y);
            ( (*pixel) < 128) ? ( (*pixel) = 0) : ( (*pixel) = 255);
        }
    }
    return bnry;
}
/*Inverts gray-scale image*/
Mat iminvert(Mat im) {
    Mat nvrt = im.clone();
    for (int x = 0; x < nvrt.rows; x++) {
        for (int y = 0; y < nvrt.cols; y++) {
            uchar* pixel = &nvrt.at<uchar>(x,y);
            (*pixel) = 255 - (*pixel);
        }
    }
    return nvrt;
}

double calc_area(std::vector<std::vector<Point>> contours){

  double area = 0.0, next;

  for (int x = 0; x < contours.size(); x++) {
    next = contourArea(contours[x]);
    area += next; 

    // Show the area calculation if debug is enabled
    if (debug&&DEBUG) { printf("After %d cycles: Total %.2f, Current %.2f\n", x, area, next);}
  }
  return area;
}

int distance(int x1,int y1,int x2,int y2) {
    return (int) std::sqrt( std::pow(x2-x1,2) + std::pow(y2-y1,2));
}

int check_args(int *size, int *type, int *count, int *range, std::string *file_name, 
  std::string *output_name, int argc, char** argv){

  char option; 
  int temp;

    // Check for arguments
  if (argc == 1) {
    usage(argv[0]);
    return 0;
  }

  while ((option = getopt(argc, argv, "hus:t:c:i:o:dr:"))!= -1) {
    switch(option) {

      // Size of pixel morphing image
      case 's':
        temp = atoi(optarg); 

        if (temp < 1) {
          fprintf(stderr, "Invalid size passed. Must be larger than 1.\n");
          return 1;
        }

        *size = temp;
        break; 

      // Type of pixel for morphing image
      case 't':
        temp = atoi(optarg); 

        if (temp == MORPH_RECT || temp == MORPH_ELLIPSE || temp == MORPH_CROSS)
          *type = temp;
        else {
          fprintf(stderr, "Invalid type passed. Try again.\n");
          return 1;
        }

        break;

      // Number of erosions to execute
      case 'c':
        temp = atoi(optarg); 

        if (temp < 1){
          fprintf(stderr, "Invalid count passed. Needs to be greater than 1.\n");
          return 1;
        }

        *count = temp;
        break;

       // Number of erosions to execute
      case 'r':
        temp = atoi(optarg); 

        if (temp < 1){
          fprintf(stderr, "Invalid radius passed. Needs to be greater than 0.\n");
          return 1;
        }
        *range = temp;
        break;

      // Input/output filenames
      case 'i':
        *file_name = std::string(optarg);
        break;

      // Debug enabled
      case 'd':
        debug = 1;
        break;

      case 'o':
        *output_name = std::string(optarg);
        break;
      // USage or help requested
      case 'u':
      case 'h':
        usage(argv[0]);
        return -1;

      default:
        usage(argv[0]);
        return 1;
    }
  } // End getopts while loop


  return 0;
}

void usage(char* binary)  {
  printf("usage: %s -s # -c # -t # -i filename -o filename\n", binary);
  printf("\t-s Kernel size used during erosion, defaults to 5\n");
  printf("\t-c Number of erosions to run, defaults to 5\n");
  printf("\t-t Kernel type to use, defaults to MORPH_ELLIPSE\n");
  printf("\t\t MORPH_RECT = 0, MORPH_CROSS = 1, MORPH_ELLIPSE = 2\n");
  printf("\t-i Input image filename\n");
  printf("\t-o Output image filename\n");
  printf("\t-r Radius of desired sample locations, defaults to 10\n");
}
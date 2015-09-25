#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

using namespace cv;

// cmake . && make && ./BlobDetector -i ../../input/st_2472776818_vAUTOLABEL.ppm -o test.png
// Erode then Dilate repeatedly
  // Look at comparing output from erosion and dilation to generate contours

// Prototypes
Mat erosion(Mat img, int count, int size, int type);
Mat find_contours(Mat img, Mat gray);
int check_args(int *size, int *type, int *count, std::string *file_name, 
  std::string *output_name, int argc, char** argv); 
void usage(char* binary);

/** @function main */
int main( int argc, char** argv ){
  std::string file_name, output_name;
  int size = 5, type = MORPH_ELLIPSE, count = 5, status;
  Mat src, gray_scale, output;

  // Check the passed arguments
  status = check_args(&size, &type, &count, &file_name, &output_name, argc, argv);
  // Usage or help requested
  if (status == -1) 
    return 0;
  // Invalid arguments passed
  if (status == 1)
    return 1;

  // Read the iamge
  src = imread(file_name);

  // Ensure the image loaded
  if( !src.data){
    fprintf(stderr, "Unable to load %s. Verify the path and try again.\n", argv[1]); 
    return 1;
  }

  // Threshold the image - BGR
  inRange(src, Scalar(25,25,150), Scalar(255,255,255), gray_scale);

  // Erode the image
  src = erosion(gray_scale, count, size, type);\

  // Save the eroded image for further comparisons
  imwrite(output_name, src);

  output = find_contours(src, gray_scale);

  imshow( "Result", output);
  waitKey(0);

  return 0;
}

/*
  Erodes the passed image multiple times determined by the value
  passed in count. The size determines the size of the pixel used
  to morph the original image. Type determines the type of pixel used.
  It expects MORPH_RECT, MORPH_CROSS, or MORPH_ELLIPSE for the type.

  After eroding multiple times, will dilate once to smooth out the contours. 
*/
Mat erosion(Mat src, int count, int size, int type){

  Mat img; 

  // Set the morph values for erosion
  Mat morph_values = getStructuringElement(type, Size(size, size));
  img = src.clone();

  // Morph the image 
  for (int x = 0; x < count; x++){
    erode(img, img, morph_values);
    
  dilate(img, img, morph_values);
  }

  // One last smooth before displaying and returning
  imshow("After Erosion", img);
  waitKey(0);

  return img;
}

/*
  Reads in an eroded image (img) and the thresholded original image(gray). 
  Finds the contours in the eroded image then draws the contours over the 
  thresholded image converted back to RGB. 
*/
Mat find_contours(Mat img, Mat gray) {

  std::vector<std::vector<Point> > contours;
  std::vector<Vec4i> hierarchy;
  Mat output; 

  // Find edges with canny 
  Canny (img, img, 100, 200, 3); 
 
  // Find contours
  findContours(img, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0,0));

  cvtColor(gray, img, CV_GRAY2RGB);

  RNG rng(12345);
  for (int x = 0; x < contours.size(); x++) {
    Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
    
      drawContours( img, contours, x, color, 1, 8, hierarchy);
  }

  return img;
}

int check_args(int *size, int *type, int *count, std::string *file_name, 
  std::string *output_name, int argc, char** argv){

  char option; 
  int temp;

    // Check for arguments
  if (argc == 1) {
    usage(argv[0]);
    return 0;
  }

  while ((option = getopt(argc, argv, "hus:t:c:i:o:"))!= -1) {
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

      // Input/output filenames
      case 'i':
        *file_name = std::string(optarg);
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
}
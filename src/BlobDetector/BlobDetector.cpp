#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <iostream>

using namespace cv;

#define DEBUG 1     // Set to 0 for release
#define PI 3.1415

// cmake . && make && ./BlobDetector -i ../../input/st_2472776818_vAUTOLABEL.ppm -o test.png

// Global debug flag
int debug = 0;

// Function prototypes
Mat smooth_input(Mat img, int count, int size, int type);
Mat region_filling(Mat image, int* size, int range);
int calcArea(int radius);
int check_args(int *size, int *type, int *count, int *range, std::string *file_name,
               std::string *output_name, int argc, char** argv);
void usage(char* binary);

/** @function main */
int main( int argc, char** argv ){
    std::string file_name, output_name;
    int size = 5, type = MORPH_ELLIPSE, count = 5, status, range = 10;
    Mat src, gray_scale, output;
    std::vector<std::vector<Point>> contours;
    int area = 0;
    
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
    if (debug&&DEBUG) { imshow("After Erosion", src); waitKey(0); }
    
    // Fill in regions; black if too small, grey otherwise
    src = region_filling(src, &area, range);
    if (debug&&DEBUG) { imshow( "Result", src); waitKey(0); }

    printf("Total Usable Area: %.02f%%\n", (double) area * 100.0/ (src.rows * src.cols));
    
    // Save the final image for further comparisons
    imwrite(output_name, src);
    return 0;
}

int calcArea(int radius) {
    return (int)PI*radius*radius;
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
    
    return img;
}

/*
 * Scan the image for safe pixels and uses flood fill to find safe
 * regions that stem from the safe pixel. The region is first filled
 * in to be gey, but if the region is bellow an area threshold
 * determined by range then it is refilled as black.
 */
Mat region_filling(Mat image, int *size, int range) {
    
    Mat filled = image.clone();
    
    //Loops to check every pixel
    for (int x = 0; x < filled.rows; x++) {
        for (int y = 0; y < filled.cols; y++) {
            
            //pixel holds greyscale value of current pixel white==255 black==0
            uchar* pixel = &filled.at<uchar>(x,y);
            if ((*pixel) == 255) {
                
                //a is the pixel area filled in by floodFill
                int a = floodFill(filled, Point(y,x), 128);
                
                //areas that are too small get refilled as black
                if (a < range)
                    floodFill(filled, Point(y,x), 0);
                else
                    (*size) += a;
            }
        }
    }
    
    return filled;
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
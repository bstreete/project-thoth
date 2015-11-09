#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <iostream>

using namespace cv;
using namespace std;
#define DEBUG 1     // Set to 0 for release
#define PI 3.1415

// cmake . && make && ./BlobDetector -i ../../input/st_2472776818_vAUTOLABEL.ppm -o test.png

// Global debug flag
int debug = 0;

// Function prototypes
Mat region_filling(Mat image, int* size, int range);
int calcArea(int radius);
Mat threshold(Mat src);
void merge(const Mat &image, Mat &image2, Mat &result, string output_name);
int check_args(int *range, std::string *file_name, std::string *output_name, 
    std::string *original_image, int argc, char** argv);
void usage(char* binary);

/** @function main */
int main( int argc, char** argv ){
    std::string overlay_name, output_name, original_name;
    int status, range = 10, area = 0;
    Mat src, original, overlay, gray_scale, output_image;
    
    // Check the passed arguments
    status = check_args(&range, &overlay_name, &output_name, &original_name, argc, argv);
    // Usage or help requested
    if (status == -1) return 0;
    // Invalid arguments passed
    if (status == 1) return 1;
    
    // Read the textureCam output image and save a copy for the super imposition
    src = imread(overlay_name);
    
    original = imread(original_name); 
    //read the original image
    
    // Ensure the images loaded
    if (!src.data){ fprintf(stderr, "Unable to load the overlay.\n"); return 1; }
    if (!original.data) { fprintf(stderr, "Unable to load the original image.\n"); return 1;}

    overlay = src.clone();
    
    if (DEBUG&&debug) imshow("Original Overlay", src);
    if (DEBUG&&debug) imshow("Original Image", original);

    // Threshold the image
    gray_scale = threshold(src); 
    if (DEBUG&&debug) imshow("Threshold Result" , gray_scale); 

    // Fill in regions; black if too small, grey otherwise
    gray_scale = region_filling(gray_scale, &area, range);
    if (DEBUG&&debug) { imshow( "Result", gray_scale); waitKey(0); }

    printf("Total Usable Area: %.02f%%\n", (double) area * 100.0/ (src.rows * src.cols));
    
    merge(original, overlay, output_image, output_name);
    imshow("HazardMap", output_image);


    // Save the final image for further comparisons
    // imwrite(output_name, output_image);

    waitKey(0);
    return 0;
}

int calcArea(int radius) {
    return (int)PI*radius*radius;
}

/* Takes the inital image and thresholds it. Then the image is returned.  */
Mat threshold(Mat src) { 
    Mat result; 

    inRange(src, Scalar(0,125,0), Scalar(0,255,0), result);
    
    return result; 
}

/*
 * Scan the image for safe pixels and uses flood fill to find safe
 * regions that stem from the safe pixel. The region is first filled
 * in to be grey, but if the region is below an area threshold
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

///perfroming simple image arithmatic
//this function will superimpose two images
void merge(const Mat &originalImage, Mat &TexCamOut, Mat &HazMap, string output_name){
    
    //allocate the result image
    HazMap = originalImage.clone();
    //superimpose
    addWeighted(originalImage, 0.7, TexCamOut, 0.9, 0.0,HazMap);
    
    //save the superimposed image
    imwrite(output_name, HazMap);
}

int check_args(int *range, std::string *file_name, std::string *output_name, 
    std::string *original_image, int argc, char** argv){
    
    char option;
    int temp;
    size_t extLoc = 99;
    
    // Check for arguments
    if (argc == 1) {
        usage(argv[0]);
        return 0;
    }
        
    while ((option = getopt(argc, argv, "hi:f:o:dr:"))!= -1) {
        switch(option) {
             
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
            
            // Final output filename
            case 'f': 
                *output_name = std::string(optarg);
                break; 

            // Original image before texture cam 
            case 'o':
                *original_image = std::string(optarg);
                break;
            
            // Debug enabled
            case 'd':
                debug = 1;
                break;
                
            // Usage or help requested
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
    printf("usage: %s -r # -i filename -o filename\n", binary);
    printf("\t-i Input overlay filename - Needs to be a ppm format image. \n\t\tDo not include the extension\n");
    printf("\t-f Final output filename\n");
    printf("\t-o Original Image - Prior to processing by Texture Cam\n"); 
    printf("\t-r Radius of desired sample locations, defaults to 10\n");
}
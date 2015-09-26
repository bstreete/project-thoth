#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <limits>
#include <iostream>

using namespace cv;

int distance(int x1,int y1,int x2,int y2) {
    return (int) std::sqrt( std::pow(x2-x1,2) + std::pow(y2-y1,2));
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
            break;
        }
    }
    return Point(-1, -1);
}

Point linkBlobRight(int range, Mat& input, KeyPoint blob) {
    int x1 = (int) blob.pt.x;
    int y1 = (int) blob.pt.y;
    
    for (int i = x1+1; i < input.cols; i++) {
        if (input.at<uchar>(y1,i) == 255) {
            for (int j = i+1; j < std::min(i+range, input.cols); j++) {
                if (input.at<uchar>(y1,j) == 0) {
                    return Point(j, y1);
                }
            }
            break;
        }
    }
    return Point(-1, -1);
}

Point linkBlobAbove(int range, Mat& input, KeyPoint blob) {
    int x1 = (int) blob.pt.x;
    int y1 = (int) blob.pt.y;
    
    for (int i = y1+1; i < input.rows; i++) {
        if (input.at<uchar>(i,x1) == 255) {
            for (int j = i+1; j < std::min(i+range,input.rows); j++) {
                if (input.at<uchar>(j,x1) == 0) {
                    return Point(x1, j);
                }
            }
            break;
        }
    }
    return Point(-1, -1);
}

Point linkBlobBelow(int range, Mat& input, KeyPoint blob) {
    int x1 = (int) blob.pt.x;
    int y1 = (int) blob.pt.y;
    
    for (int i = y1-1; i > 0; i--) {
        if (input.at<uchar>(i,x1) == 255) {
            for (int j = i-1; j > std::max(i-range,0); j--) {
                if (input.at<uchar>(j,x1) == 0) {
                    return Point(x1, j);
                }
            }
            break;
        }
    }
    return Point(-1, -1);
}

int main(int argc, char** argv) {
    
    int range = 20;
    std::string in_file_name = argv[1];
    
    Mat input = imread(argv[1], IMREAD_GRAYSCALE);
    
    input = imbinary(input);
    
    Mat output = input.clone();
    
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
    hzrd_detect->detect(input, hzrd_keypoints);
    
    std::string out_file_name = in_file_name.substr(0, in_file_name.length()-4) + "_out.png";
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
            line(output, Point(x1,y1), Point(x2,y2), Scalar(0,0,0), 2);
        }
        
        Point p2;
        
        /*Links blobs to above hazard zones within range*/
        p2 = linkBlobAbove(range, input, p1);
        if ( !(p2.x == -1 && p2.y == -1) ) {
            line(output, Point(x1,y1), Point(p2.x, p2.y+4), Scalar(0,0,0), 2);
            perimeterLinked = true;
        }
        
        /*Links blobs to below hazard zones within range*/
        p2 = linkBlobBelow(range, input, p1);
        if ( !(p2.x == -1 && p2.y == -1) ) {
            line(output, Point(x1,y1), Point(p2.x, p2.y-4), Scalar(0,0,0), 2);
            perimeterLinked = true;
        }
        
        /*Links blobs to right hazard zones within range*/
        p2 = linkBlobRight(range, input, p1);
        if ( !(p2.x == -1 && p2.y == -1) ) {
            line(output, Point(x1,y1), Point(p2.x+4, p2.y), Scalar(0,0,0), 2);
            perimeterLinked = true;
        }

        /*Links blobs to left hazard zones within range*/
        p2 = linkBlobLeft(range, input, p1);
        if ( !(p2.x == -1 && p2.y == -1) ) {
            line(output, Point(x1,y1), Point(p2.x-4, p2.y), Scalar(0,0,0), 2);
            perimeterLinked = true;
        }
        
        if (!perimeterLinked) {
            std::vector<Point> points = {
                linkBlobLeft(3000, input, p1),
                linkBlobRight(3000, input, p1),
                linkBlobAbove(3000, input, p1),
                linkBlobBelow(3000, input, p1)
            };
            int minDistLink = std::min(min(distance(x1, y1, points[0].x, points[0].y),
                                           distance(x1, y1, points[1].x, points[1].y)),
                                       min(distance(x1, y1, points[2].x, points[2].y),
                                           distance(x1, y1, points[3].x, points[3].y)));
            for (Point pMinDist : points) {
                if (distance(x1, y1, pMinDist.x, pMinDist.y) == minDistLink) {
                    line(output, Point(x1,y1), pMinDist, Scalar(0,0,0), 2);
                }
            }
        }
    }
    
    output = iminvert(output);
    
    /*
     *A blob detector to detect safe zones, area must be
     *larger than area of circle with diameter $range
     */
    SimpleBlobDetector::Params safe_params;
    safe_params.filterByArea = true;
    safe_params.minArea = (int) (3.14*std::pow(range/2,2));
    safe_params.maxArea = std::numeric_limits<int>::max();
    safe_params.filterByConvexity = true;
    safe_params.minConvexity = .8;
    
    /*Detect safe zones*/
    Ptr<SimpleBlobDetector> safe_detect = SimpleBlobDetector::create(safe_params);
    std::vector<KeyPoint> safe_keypoints;
    safe_detect->detect(output, safe_keypoints);
    for (KeyPoint p : safe_keypoints)
        std::cout << '(' << p.pt.x << ',' << p.pt.y << ')' << '\n';
    
    /*display circle around safe zones*/
    drawKeypoints(output, safe_keypoints, output, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    imwrite(out_file_name, output);
    return 0;
}

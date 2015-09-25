

#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace cv;
using namespace std;

///perfroming simple image arithmatic
//this function will superimpose two images
void merge(const Mat &image, Mat &image2, Mat &result){
    //allocate the result image
    result.create(image.size(), image.type());
    addWeighted(image, 0.7, image2, 0.9, 0.,result);
}


int main(int argc, const char * argv[]) {
    Mat image1 = imread("");
    Mat image2 = imread("");
    Mat result;
    
    
    merge(image1, image2, result);
    namedWindow("merge");
    imshow("merge", result);
    waitKey(0);
        return 0;
}

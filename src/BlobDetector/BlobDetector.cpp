#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <stdlib.h>
#include <stdio.h>

using namespace cv;

/** @function main */
int main( int argc, char** argv )
{

  Mat src = imread(argv[1]);
  Mat final, gray;
  std::vector<std::vector<Point> > contours;
  std::vector<Vec4i> hierarchy;

  if( !src.data )
  { return -1; }

  // Black out anything but red - BGR not RGB
  inRange(src, Scalar(0,0,150), Scalar(255,255,255), gray);

  // Find edges with canny 
  Canny (gray, final, 100, 200, 3); 
  // Find contours 

  imshow("Image", gray);
  waitKey(0);

  findContours(final, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, Point(0,0));

  Mat output = Mat::zeros(final.size(), CV_8UC3);
  RNG rng(12345);
  for (int x = 0; x < contours.size(); x++) {
    Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255));
    drawContours( output, contours, x, color, -1, 8, hierarchy, 0, Point() );
  }
  // drawContours(output, contours, -1, Scalar(100,100,0), 
  //   -1, CV_AA, hierarchy);

// void drawContours(InputOutputArray image, InputArrayOfArrays contours, int contourIdx, 
//   const Scalar& color, int thickness=1, int lineType=8, InputArray hierarchy=noArray(),
//    int maxLevel=INT_MAX, Point offset=Point() )
  imshow( "Output", output );
  // imshow( "Original", src);
  waitKey(0);

  return 0;
  }

//   /** @function thresh_callback */
// void thresh_callback(int, void* )
// {
//   Mat canny_output;
//   vector<vector<Point> > contours;
//   vector<Vec4i> hierarchy;

//   /// Detect edges using canny
//   Canny( src_gray, canny_output, thresh, thresh*2, 3 );
//   /// Find contours
//   findContours( canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

//   /// Draw contours
//   Mat drawing = Mat::zeros( canny_output.size(), CV_8UC3 );
//   for( int i = 0; i< contours.size(); i++ )
//      {
//        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
//        drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
//      }

//   /// Show in a window
//   namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
//   imshow( "Contours", drawing );
// }
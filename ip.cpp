#pragma once

#include"opencv2\opencv.hpp"
#include<iostream>
#include <vector>


using namespace cv;
using namespace std;
class SkinDetector
{
public:
    SkinDetector(void);
    ~SkinDetector(void);

cv::Mat getSkin(cv::Mat input);

private:
    int Y_MIN;
    int Y_MAX;
    int Cr_MIN;
    int Cr_MAX;
    int Cb_MIN;
   int Cb_MAX;
};



SkinDetector::SkinDetector(void)
{
Y_MIN  = 0;
Y_MAX  = 255;
Cr_MIN = 133;
Cr_MAX = 173;
Cb_MIN = 77;
Cb_MAX = 127;
}

SkinDetector::~SkinDetector(void)
{
}


cv::Mat SkinDetector::getSkin(cv::Mat input)
{
 int thresh = 35;
 int i,j;


cv::Mat skin,canny_output;
Mat dst, detected_edges;
Mat img = input.clone();
int channels = input.channels();
int nRows = input.rows;
int nCols = input.cols;
int scale_Col = 0;

//first convert our RGB image to YCrCb
cv::cvtColor(input,skin,cv::COLOR_BGR2YCrCb);
//cvtColor(input, skin, CV_BGR2HSV);
 blur( skin, skin, Size(3,3) );
// cvtColor(input, skin, CV_BGR2HSV);

cv::inRange(skin,cv::Scalar(Y_MIN,Cr_MIN,Cb_MIN),cv::Scalar(Y_MAX,Cr_MAX,Cb_MAX),skin);

     threshold( skin,dst, 100, 255, THRESH_BINARY);

    imshow("zcrcb Image",dst);
//waitKey();


 vector<vector<Point> > contours;
 vector<vector<Point> > contours_new;
 vector<Vec4i> hierarchy;
 Canny( dst, canny_output, thresh, thresh*2, 3 );
 imshow("cannz Image",canny_output);
 findContours( canny_output, contours, hierarchy, RETR_EXTERNAL , CHAIN_APPROX_SIMPLE, Point(0, 0) );
int height =0;
int width = 0;
 vector<RotatedRect> minRect( contours.size() );
 for( size_t i = 0; i < contours.size(); i++ )
      minRect[i] = minAreaRect( Mat(contours[i]) );
 for( size_t i = 0; i< contours.size(); i++ ) {
     if(contours[i].size()> 90) { //we only want to keep big contour
         Scalar color = Scalar( 255,0,255);
         height = minRect[i].size.height;
         width = minRect[i].size.width ;
         Point2f rect_points[4]; minRect[i].points( rect_points );
                   //drawing rectangle
         if(height>width){
             minRect[i].size.height =(float)(0.33)*minRect[i].size.height;
             minRect[i].center = (rect_points[1]+rect_points[2])/2 + (rect_points[0]-rect_points[1])/6;
         } else {
             minRect[i].size.width =(float)(0.33)*minRect[i].size.width;
             minRect[i].center = (rect_points[2]+rect_points[3])/2 + (rect_points[0]-rect_points[3])/6;
         }
         minRect[i].points( rect_points );
         for( int j = 0; j < 4; j++ )
             line( input, rect_points[j], rect_points[(j+1)%4], color, 2, 8 );

     }
}


return input;
}

int main()
{
VideoCapture capture;
//open capture object at location zero (default location for webcam)

capture.open(0);


capture.set(CV_CAP_PROP_FRAME_WIDTH,320);
capture.set(CV_CAP_PROP_FRAME_HEIGHT,480);

Mat cameraFeed;

SkinDetector mySkinDetector;

Mat skinMat;
//cameraFeed = imread("hand.jpg");
while(1){


capture.read(cameraFeed);

skinMat= mySkinDetector.getSkin(cameraFeed);

imshow("Skin Image",skinMat);
waitKey(16);

}
return 0;
}

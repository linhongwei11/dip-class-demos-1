#pragma once
#include <opencv.hpp>


using namespace cv;


//gamma矫正演示函数
int gammaMain();



//gamma矫正功能函数
int gammaCorrection(cv::Mat srcMat, cv::Mat & dstMat, float gamma);



#pragma once
#include <opencv.hpp>


using namespace cv;


//gamma������ʾ����
int gammaMain();



//gamma�������ܺ���
int gammaCorrection(cv::Mat srcMat, cv::Mat & dstMat, float gamma);



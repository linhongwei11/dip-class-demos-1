#pragma once
#include <opencv.hpp>


using namespace cv;
using namespace std;


//gamma������ʾ����
int gammaMain();
//ֱ��ͼ���⻯��ʾ����
int equalizeMain();



//gamma�������ܺ���
int gammaCorrection(cv::Mat srcMat, cv::Mat & dstMat, float gamma);


//ֱ��ͼ���ܺ���
int drawSimpleHist(cv::Mat & histMat, float * srcHist);
int calcSimpleHist(const cv::Mat src, float * dstHist);
int calcSimpleHistWithMask(const cv::Mat src, const cv::Mat mask, float * dstHist);

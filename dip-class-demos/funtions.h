#pragma once
#include <opencv.hpp>

using namespace cv;
using namespace std;

void on_mouse(int EVENT, int x, int y, int flags, void* userdata);
int drawHist(cv::Mat & histMat, float * srcHist, int bin_width, int bin_heght);


int verifyGaussian();
int bgSub_demo();


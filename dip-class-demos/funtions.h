#pragma once
#include <opencv.hpp>

using namespace cv;
using namespace std;



VideoCapture createInput(bool useCamera, std::string videoPath);

//ģ��ƥ��demo
int templateMatching_demo();


//��������good feature to track
int opticalFlow_demo();






#include "stdafx.h"
#include "funtions.h"
#include <stdio.h>
#include <direct.h>

using namespace std;

//OpenCV中几种读入数据的方法
void readImage()
{


	//00.读入单张图片，路径可替换成自己的图片的路径
	cv::Mat srcMat = imread("../testImages\\rim.png");

	//01.读入单张图片，加参数0，表示读入，并转换成灰度图
	cv::Mat gryMat = imread("../testImages\\rim.png",0);



	//显示图片
	imshow("src",srcMat);
	imshow("gray",gryMat);

	//显示图片，必须要加waitKey()，否则无法显示图像
	//waitKey(0),无限地显示窗口，直到任何按键按下
	//如果是其他数字，如waitKey(25)表示25毫秒，然后关闭。
	waitKey(0);


	VideoCapture cap;

}

﻿#include "stdafx.h"
#include "funtions.h"
#include <stdio.h>

using namespace std;


void readImage()
{
	//读入单张图片，路径可替换成自己的图片的路径
	cv::Mat srcMat = imread("../testImages\\butterfly.jpg");

	//读取图片的一些信息
	// Mat是否为空，可以判断读图是否成功
	std::cout << "empty:" << (srcMat.empty() ? "the Mat is empty,fail to read" : "not empty") << std::endl;
	if (srcMat.empty())return;

	//在Mat中
	//cols 是 列数 相当于 width  
	//rows 是 行数 相当于 height 
	//行数
	std::cout << "rows:" << srcMat.rows << std::endl;
	//列数 
	std::cout << "cols:" << srcMat.cols << std::endl;
	//维度，普通图片为2维
	std::cout << "dims:" << srcMat.dims << std::endl;

	// Size是OpenCV内部定义的数据类型
	std::cout << "size[]:" << srcMat.size().width << "," << srcMat.size().height << std::endl;

	// 深度id
	// 用来度量每一个像素中【每一个通道】的精度，depth数值越大，精度越高。在                 
	//Opencv中，Mat.depth()得到的是一个0~6的数字，分别代表不同的位数，对应关系如下：                            
	//opencv中，由于使用Mat.at访问数据时，必须正确填写相应的数据类型，
	//Mat的类型定义方法
	//The definition is as follows:
	//CV_(位） + （数据类型） + （通道数量）
	//如, CV_32FC1 表示 32位 float型单通道,
	//OpenCV中的数据类型与C的数据类型的对应关系。
	/*
	uchar   CV_8U		0 
	char    CV_8S		1
	ushort  CV_16U		2
	short   CV_16S		3	
	int     CV_32S		4
	float   CV_32F		5
	double  CV_64F		6
	*/
	std::cout << "depth (ID):" << srcMat.depth() << std::endl;


	// channel数，如灰度图为单通道，RGB图为3通道
	std::cout << "channels:" << srcMat.channels() << std::endl;

	// Mat中一个元素的size(byte数),矩阵一个元素占用的字节数，
	//数据类型是
	//CV_8UC1，				elemSize==1，1 byte；
	//CV_8UC3/CV_8SC3，		elemSize==3；3 byte
	//CV_16UC3/CV_16SC3，	elemSize==6；6 byte
	//即elemSize==字节数x通道数；矩阵一个元素占用的字节数，
	std::cout << "elemSize:" << srcMat.elemSize() << "[byte]" << std::endl;
	// Mat中一个元素的一个通道的size(byte数)，矩阵元素一个通道占用的字节数,
	//eleSize1==elemSize/channels；
	std::cout << "elemSize1 (elemSize/channels):" << srcMat.elemSize1() << "[byte]" << std::endl;

	//元素的总数，如果是图像，即为像素个数
	std::cout << "total:" << srcMat.total() << std::endl;
	// step （byte数）  
	//Mat矩阵中每一行的“步长”，以字节为基本单位，每一行中所有元素的字节总量
	//cols*elemSize=cols*eleSize1*channels
	std::cout << "step:" << srcMat.step << "[byte]" << std::endl;
	// 一个step的channel总数，每行的channel数
	std::cout << "step1 (step/elemSize1):" << srcMat.step1() << std::endl;
	// 该Mat在内存上是否连续
	std::cout << "isContinuous:" << (srcMat.isContinuous() ? "true" : "false") << std::endl;
	// 是否为子矩阵
	std::cout << "isSubmatrix:" << (srcMat.isSubmatrix() ? "true" : "false") << std::endl;


	//读入单张图片，加参数0，表示读入，并转换成灰度图
	cv::Mat gryMat = imread("../testImages\\butterfly.jpg", 0);
	if (srcMat.empty())return;
	//保存图片
	imwrite("../testImages\\gray-butterfly.jpg",gryMat);

	//显示图片
	imshow("src", srcMat);
	imshow("gray", gryMat);

	//显示图片，必须要加waitKey()，否则无法显示图像
	//waitKey(0),无限地显示窗口，直到任何按键按下
	//如果是其他数字，如waitKey(25)表示25毫秒，然后关闭。
	waitKey(0);
}

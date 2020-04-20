#include "stdafx.h"
#include "funtions.h"
#include <stdio.h>

using namespace std;

//OpenCV�м��ֶ������ݵķ���
void readImage()
{


	//00.���뵥��ͼƬ��·�����滻���Լ���ͼƬ��·��
	cv::Mat srcMat = imread("../testImages\\rim.png");

	//01.���뵥��ͼƬ���Ӳ���0����ʾ���룬��ת���ɻҶ�ͼ
	cv::Mat gryMat = imread("../testImages\\rim.png",0);



	//��ʾͼƬ
	imshow("src",srcMat);
	imshow("gray",gryMat);

	//��ʾͼƬ������Ҫ��waitKey()�������޷���ʾͼ��
	//waitKey(0),���޵���ʾ���ڣ�ֱ���κΰ�������
	//������������֣���waitKey(25)��ʾ25���룬Ȼ��رա�
	waitKey(0);


	VideoCapture cap;

}

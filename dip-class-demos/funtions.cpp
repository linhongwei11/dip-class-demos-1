#include "stdafx.h"
#include <opencv.hpp>
#include "funtions.h"

using namespace cv;
using namespace std;


//blob������ʾ������
//�ڰ��ܣ���ϰ1������е�Բ�׶�λ
void rimBlobAnalysis()
{

	float wh_ratio_high = 1.1;
	float wh_ratio_low = 0.9;
	float area_th = 100;

	//����ͼ������
	Mat srcMat;
	Mat bnyMat;
	Mat disMat;
	Mat sttMat;
	Mat cntMat;
	Mat lblMat;

	//��ȡͼƬ
	srcMat=imread("D:\\rim.png");
	srcMat.copyTo(disMat);
	cvtColor(srcMat,srcMat,COLOR_BGR2GRAY);

	//��ֵ��
	threshold(srcMat,bnyMat,100,255,THRESH_OTSU);

	//��ɫ
	bnyMat = 255 - bnyMat;

	//��ͨ��
	int nComp = connectedComponentsWithStats(bnyMat, lblMat, sttMat, cntMat);

	int * flag = new int[nComp];
	memset(flag, 0, sizeof(int)*nComp);

	//��������
	//0��Ϊ������������i=1��ʼѭ��
	for (int i = 1; i < nComp; i++) {
		float width = (float)sttMat.at<int>(i, CC_STAT_WIDTH);
		float height= (float)sttMat.at<int>(i, CC_STAT_HEIGHT);
		float ratio = width/height;

		if (
			   (ratio > wh_ratio_low)
			&& (ratio < wh_ratio_high)
			&& (sttMat.at<int>(i, CC_STAT_AREA) > area_th)
			)
		{
			//����bounding box
			Rect bbox;
			//bounding box���Ͻ�����
			bbox.x = sttMat.at<int>(i, 0);
			bbox.y = sttMat.at<int>(i, 1);
			//bouding box�Ŀ�ͳ� 
			bbox.width = sttMat.at<int>(i, 2);
			bbox.height = sttMat.at<int>(i, 3);
			//����
			rectangle(disMat, bbox, CV_RGB(255, 255, 0), 2, 8, 0);

			//��¼��ǩ
			flag[i] = 1;
		}
	}

	//ɸѡ����������ɫ
	for (int i = 0; i<srcMat.rows; i++)
	{
		for (int j = 0; j<srcMat.cols; j++)
		{
			int no = lblMat.at<int>(i, j);

			if (flag[no] == 1) {
				disMat.at<Vec3b>(i, j)[0] = 0;
				disMat.at<Vec3b>(i, j)[1] = 255;
				disMat.at<Vec3b>(i, j)[2] = 255;
			}
		}
	}

	delete[] flag;

	imshow("source image",srcMat);
	imshow("binary image",bnyMat);
	imshow("hole image",disMat);

	waitKey(0);

	destroyAllWindows();

}


//�ڰ��ܣ���ϰ2��оƬ��λ
void chipBlobAnalysis()
{
	//�������ֵ
	float wh_ratio_high = 1.2;
	float wh_ratio_low = 0.9;

	//����ͼ������
	Mat srcMat;
	Mat bnyMat;
	Mat disMat;
	Mat sttMat;
	Mat cntMat;
	Mat lblMat;

	//��ȡͼƬ
	srcMat = imread("F:\\die_on_chip.png");
	srcMat.copyTo(disMat);
	cvtColor(srcMat, srcMat, COLOR_BGR2GRAY);

	//��ֵ��
	threshold(srcMat, bnyMat, 100, 255, THRESH_OTSU);

	//��ʴ����
	cv::Mat element = cv::Mat::ones(3,3, CV_8UC1);
	erode(bnyMat,bnyMat, element, cv::Point(-1, -1));

	//ͨ��findContours����Ѱ����ͨ��
	vector<vector<Point>> contours;
	findContours(bnyMat,contours,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);


	//��������
	for (int i = 0; i < contours.size(); i++) {
		//�����С����ı���
		RotatedRect rbox = minAreaRect(contours[i]);

		//��������
		float width = (float)rbox.size.width;
		float height= (float)rbox.size.height;
		float ratio = width / height;

		//����ɸѡ
		if (
				(ratio > wh_ratio_low)
			&&	(ratio < wh_ratio_high)
			)
		{
			//��������
			drawContours(disMat, contours, i, Scalar(0,255,255), 1, 8);
			//��ȡ4������
			cv::Point2f vtx[4];
			rbox.points(vtx);
			//����4����
			for (int i = 0; i < 4; ++i) {
				cv::line(disMat, vtx[i], vtx[i<3 ? i + 1 : 0], cv::Scalar(0, 0, 255), 2, CV_AA);
			}
		}
	}

	imshow("source image", srcMat);
	imshow("binary image", bnyMat);
	imshow("hole image", disMat);

	waitKey(0);

	destroyAllWindows();

}


//�ڰ��ܣ���ϰ3����Ƭ�еı��Ƕ�λ
void libBlobAnalysis()
{
	//ɸѡ
	int width_th = 50;
	int height_th = 50;

	//0-180
	//��ɫ
	//��һ��Hue�ķ�Χ
	double i_minH = 0;
	double i_maxH = 20;
	//�ڶ���Hue�ķ�Χ
	double i_minH2 = 160;
	double i_maxH2 = 180;

	//0-255
	double i_minS = 100;
	double i_maxS = 255;
	//0-255
	double i_minV = 50;
	double i_maxV = 255;

	
	Mat srcMat = imread("F:\lib.jpg");
	Mat hsvMat;
	Mat disMat;
	Mat rangeMat1;
	Mat rangeMat2;
	Mat bnyMat;
	Mat lblMat, sttMat, cntMat;

	//ת����hsvģʽ
	cvtColor(srcMat, hsvMat, COLOR_BGR2HSV);
	srcMat.copyTo(disMat);

	cv::inRange(hsvMat, Scalar(i_minH, i_minS, i_minV), Scalar(i_maxH, i_maxS, i_maxV), rangeMat1);
	cv::inRange(hsvMat, Scalar(i_minH2, i_minS, i_minV), Scalar(i_maxH2, i_maxS, i_maxV), rangeMat2);

	//�����㣬�ϲ�������Χ��ɸѡ���
	bnyMat = rangeMat1 + rangeMat2;

	//��ͨ��
	int nComp = connectedComponentsWithStats(bnyMat, lblMat, sttMat, cntMat);

	//0��Ϊ������������i=1��ʼѭ��
	for (int i = 1; i < nComp;i++) {
		Rect bbox;
		//bounding box���Ͻ�����
		bbox.x = sttMat.at<int>(i, 0);
		bbox.y = sttMat.at<int>(i, 1);
		//bouding box�Ŀ�ͳ� 
		bbox.width = sttMat.at<int>(i, 2);
		bbox.height = sttMat.at<int>(i, 3);
		//����
		if (
				bbox.width > width_th
			&&	bbox.height > height_th
			) 
		{
			rectangle(disMat, bbox, CV_RGB(255, 255, 0), 2, 8, 0);
		}
	}
	
	imshow("source image", srcMat);
	imshow("binary image", bnyMat);
	imshow("hole image", disMat);

	waitKey(0);

}

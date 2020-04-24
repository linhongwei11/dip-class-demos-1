#include "stdafx.h"
#include "funtions.h"
#include <math.h>

using namespace cv;
using namespace std;

//手动实现，通过 HOG (Histogram-of-Oriented-Gradients)比较图像相似度
int compareImages()
{

	//读入图像
	cv::Mat refMat	= imread("../testImages\\hogTemplate.jpg",0);
	cv::Mat plMat	= imread("../testImages\\img1.jpg", 0);
	cv::Mat bgMat	= imread("../testImages\\img2.jpg", 0);

	if (refMat.empty() || plMat.empty() || bgMat.empty()) {
		std::cout << "failed to read image!:" << std::endl;
		return -1;
	}

	//参数设置
	int nAngle = 8;
	int blockSize = 16;
	int nX = refMat.cols / blockSize;
	int nY = refMat.rows / blockSize;
	
	int bins = nX*nY*nAngle;


	float * ref_hist = new float[bins];
	memset(ref_hist, 0, sizeof(float)*bins);
	float * pl_hist = new float[bins];
	memset(pl_hist, 0, sizeof(float)*bins);
	float * bg_hist = new float[bins];
	memset(bg_hist, 0, sizeof(float)*bins);

	int reCode = 0;
	//计算三张输入图片的HOG
	reCode = calcHOG(refMat, ref_hist, nAngle, blockSize);
	reCode = calcHOG(plMat, pl_hist, nAngle, blockSize);
	reCode = calcHOG(bgMat, bg_hist, nAngle, blockSize);

	if (reCode != 0) {
		return -1;
	}

	//计算直方图距离
	float dis1 = normL2(ref_hist, pl_hist, bins);
	float dis2 = normL2(ref_hist,bg_hist , bins);

	std::cout << "distance between reference and img1:" << dis1 << std::endl;
	std::cout << "distance between reference and img2:" << dis2 << std::endl;

	(dis1<=dis2) ? (std::cout << "img1 is similar" << std::endl) : (std::cout << "img2 is similar"<<std::endl);


	imshow("ref",refMat);
	imshow("img1",plMat);
	imshow("img2",bgMat);

	waitKey(0);

	delete[] ref_hist;
	delete[] pl_hist;
	delete[] bg_hist;
	destroyAllWindows();

	return 0;
}

//手动实现 HOG (Histogram-of-Oriented-Gradients) 
int calcHOG(cv::Mat src, float * hist,int nAngle,int cellSize)
{

	if (cellSize> src.cols || cellSize> src.rows) {
		return -1;
	}

	//参数设置
	int nX = src.cols / cellSize;
	int nY = src.rows / cellSize;

	int binAngle = 360 / nAngle;

	//计算梯度及角度
	Mat gx, gy;
	Mat mag, angle;
	Sobel(src, gx, CV_32F, 1, 0, 1);
	Sobel(src, gy, CV_32F, 0, 1, 1);
	// x方向梯度，y方向梯度，梯度，角度，决定输出弧度or角度
	// 默认是弧度radians，可以选择角度degrees.
	cartToPolar(gx, gy, mag, angle, true);

	cv::Rect roi;
	roi.x = 0;
	roi.y = 0;
	roi.width = cellSize;
	roi.height = cellSize;

	for (int i = 0; i < nY; i++) {
		for (int j = 0; j < nX; j++) {
			
			cv::Mat roiMat;
			cv::Mat roiMag;
			cv::Mat roiAgl;

			roi.x = j*cellSize;
			roi.y = i*cellSize;

			//赋值图像
			roiMat = src(roi);
			roiMag = mag(roi);
			roiAgl = angle(roi);

			//当前cell第一个元素在数组中的位置
			int head = (i*nX + j)*nAngle;

			for (int n = 0; n < roiMat.rows; n++) {
				for (int m = 0; m < roiMat.cols; m++) {
					//计算角度在哪个bin，通过int自动取整实现
					int pos =(int)(roiAgl.at<float>(n, m) / binAngle);
					hist[head+pos] += roiMag.at<float>(n, m);
				}
			}
		
		}
	}

	return 0;
}

float normL2(float * Hist1, float * Hist2, int size)
{
	float sum = 0;
	for (int i = 0; i < size; i++) {
		sum += (Hist1[i] - Hist2[i])*(Hist1[i] - Hist2[i]);
	}
	sum = sqrt(sum);
	return sum;
}



int detectPeople()
{

	VideoCapture cap("../testImages\\vtest.avi");

	//如果视频打开失败
	if (!cap.isOpened()) {
		std::cout << "Unable to open video!" << std::endl;
		return -1;
	}

	Mat frame;

	HOGDescriptor hog;
	hog.setSVMDetector(HOGDescriptor::getDefaultPeopleDetector());

	while (1) {

		cap >> frame;

		std::vector<cv::Rect> found;
		
		hog.detectMultiScale(frame, found, 0.2, cv::Size(8, 8), cv::Size(16, 16), 1.05, 2);

		std::vector<cv::Rect>::const_iterator it;

		std::cout << "found:" << found.size() << std::endl;

		for (it = found.begin(); it != found.end(); ++it) {
			cv::Rect r = *it;
			r.x += cvRound(r.width*0.1);
			r.width = cvRound(r.width*0.8);
			r.y += cvRound(r.height*0.07);
			r.height = cvRound(r.height*0.8);
			cv::rectangle(frame, r.tl(), r.br(), cv::Scalar(0, 255, 0), 3);
		}

		cv::namedWindow("result", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
		cv::imshow("result", frame);
		cv::waitKey(10);

		

	}


	return 0;
}

int harrisDetector()
{

//	VideoCapture cap(0);
	VideoCapture cap("../testImages\\vtest.avi");

	//如果视频打开失败
	if (!cap.isOpened()) {
		std::cout << "Unable to open camera!" << std::endl;
		return -1;
	}

	Mat dst, dst_norm, dst_norm_scaled;

	//参数设置
	int blockSize = 2;
	int apertureSize = 3;
	double k = 0.04;

	Mat frame;
	Mat gray;

	int thresh = 200;

	while (1) {

		cap >> frame;

		cvtColor(frame,gray,CV_RGB2GRAY);

		Mat dst = Mat::zeros(frame.size(), CV_32FC1);

		cornerHarris(gray, dst, blockSize, apertureSize, k, BORDER_DEFAULT);

		//归一化
		normalize(dst, dst_norm, 0, 255, NORM_MINMAX, CV_32FC1, Mat());
		convertScaleAbs(dst_norm, dst_norm_scaled);

		//标记超过阈值的像素
		for (int j = 0; j < dst_norm.rows; j++)
		{
			for (int i = 0; i < dst_norm.cols; i++)
			{
				if ((int)dst_norm.at<float>(j, i) > thresh)
				{
					circle(dst_norm_scaled, Point(i, j), 5, Scalar(0), 2, 8, 0);
				}
			}
		}

		imshow("corner", dst_norm_scaled);

		waitKey(30);
	}

	return 0;
}

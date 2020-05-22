#include "stdafx.h"
#include "funtions.h"


//观察的位置
cv::Point vP;

void threshold_track(int, void *)//这里就是定义的一个回调函数，里面是canny相关的操作
{
	//Mat result;
	//threshold(subMat, diff_thresh, 100, 255, CV_THRESH_BINARY);
	//imshow("边缘检测", result);
}


//该demo验证并演示，视频中的像素灰度值变换是否呈高斯分布
int verifyGaussian()
{
	//----------------------读取视频文件--------------------------
	//VideoCapture capVideo("../testImages\\vtest.avi");
	VideoCapture capVideo(0);

	//如果视频打开失败
	if (!capVideo.isOpened()) {
		std::cout << "Unable to open video!" << std::endl;
		return -1;
	}

	int cnt = 0;
	int bin_width = 3;
	int bin_heght = 50;
	float histgram[256] = {0};

	cv::Mat histMat;

	while (1) {

		Mat frame;
		Mat grayMat;
		capVideo >> frame;

		if (frame.empty()) {
			std::cout << "Unable to read frame!" << std::endl;
			return -1;
		}

		//第一帧选取像素
		if (cnt == 0) {
			Mat selectMat;
			frame.copyTo(selectMat);
			namedWindow("mouseCallback");
			imshow("mouseCallback", selectMat);
			setMouseCallback("mouseCallback", on_mouse, &selectMat);
			waitKey(0);
			destroyAllWindows();
		}

		cvtColor(frame,grayMat,COLOR_BGR2GRAY);

		//获得像素灰度值
		int index = grayMat.at<uchar>(vP.y,vP.x);
		//直方图相应的bin加1
		histgram[index]++;

		//绘制直方图
		drawHist(histMat, histgram, bin_width,bin_heght);

		drawMarker(frame, vP, Scalar(255, 255, 255));
		imshow("frame",frame);
		imshow("histMat",histMat);

		waitKey(30);
		cnt++;
	}

	return 0;
}


int bgSub_demo()
{
	//----------------------读取视频文件--------------------------
	VideoCapture capVideo(0);
	//VideoCapture capVideo("../testImages\\vtest.avi");

	//如果视频打开失败
	if (!capVideo.isOpened()) {
		std::cout << "Unable to open video!" << std::endl;
		return -1;
	}

	//计数器
	int cnt = 0;

	Mat frame;
	Mat bgMat;
	Mat subMat;
	Mat bny_subMat;

	while (1) {

		capVideo >> frame;
		cvtColor(frame,frame,COLOR_BGR2GRAY);

		if (cnt== 0) {
			//第一帧，获得背景图像
			frame.copyTo(bgMat);
		}
		else {
			//第二帧开始背景差分
			//背景图像和当前图像相减
			absdiff(frame, bgMat, subMat);
			//差分结果二值化
			threshold(subMat, bny_subMat, 100, 255, CV_THRESH_BINARY);

			imshow("b_subMat",bny_subMat);
			imshow("subMat",subMat);

			waitKey(30);
		}

		cnt++;
	}

	return 0;
}

int bgSubGaussian_demo()
{
	//----------------------读取视频文件--------------------------
	VideoCapture capVideo(0);
	//VideoCapture capVideo("../testImages\\vtest.avi");

	//如果视频打开失败
	if (!capVideo.isOpened()) {
		std::cout << "Unable to open video!" << std::endl;
		return -1;
	}

	//用来建立背景模型的数量
	int nBg = 20;

	//用来计算背景模型的图像
	std::vector<cv::Mat> srcMats;

	int cnt = 0;
	cv::Mat frame;
	cv::Mat bgMat;
//	cv::Mat srcMat;

	while (true)
	{
		capVideo >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);

		//前面的nBg帧，计算背景
		if (cnt < nBg) {
			srcMats.push_back(frame);
		}
		else if (cnt == nBg) {
			//计算模型
			calcGausianBackground(srcMats,bgMat);
			break;
		}
		else {
			//背景差分

		}

		cnt++;

	}


	for (int i = 0; i < srcMats.size(); i++) {


	}

	return 0;
}

int calcGausianBackground(std::vector<cv::Mat> srcMats, cv::Mat & bgMat)
{
	return 0;
 }


//鼠标响应函数
void on_mouse(int EVENT, int x, int y, int flags, void* userdata)
{

	Mat hh;
	hh = *(Mat*)userdata;
	switch (EVENT)
	{
	case EVENT_LBUTTONDOWN:
	{
		vP.x = x;
		vP.y = y;
		drawMarker(hh,vP,Scalar(255,255,255));
		//circle(hh, vP, 4, cvScalar(255, 255, 255), -1);
		imshow("mouseCallback", hh);
		return;
	}
	break;
	}

}

//绘制直方图
int drawHist(cv::Mat & histMat, float * srcHist, int bin_width, int bin_heght)
{
	histMat.create(bin_heght, 256 * bin_width, CV_8UC3);

	histMat = Scalar(255, 255, 255);

	float maxVal = *std::max_element(srcHist, srcHist + 256);

	for (int i = 0; i < 256; i++) {
		Rect binRect;
		binRect.x = i*bin_width;
		float height_i = (float)bin_heght*srcHist[i] / maxVal;
		binRect.height = (int)height_i;
		binRect.y = bin_heght - binRect.height;
		binRect.width = bin_width;
		rectangle(histMat, binRect, CV_RGB(255, 0, 0), -1);
	}

	return 0;
}

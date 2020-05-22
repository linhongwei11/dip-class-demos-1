#include "stdafx.h"
#include "funtions.h"


//观察的位置
cv::Point vP;

int sub_threshold = 0;
Mat bgMat;
Mat subMat;
Mat bny_subMat;

void threshold_track(int, void *)//这里就是定义的一个回调函数，里面是canny相关的操作
{

	threshold(subMat, bny_subMat,sub_threshold , 255, CV_THRESH_BINARY);
	imshow("Result", bny_subMat);
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
	int bin_heght = 100;
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

			namedWindow("Result", WINDOW_AUTOSIZE);
			//滑动条创建
			cv::createTrackbar("threshold", "Result", &sub_threshold, 255, threshold_track);
			threshold_track(0,0);

			imshow("frame",frame);
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

	//用来计算背景模型的图像
	std::vector<cv::Mat> srcMats;

	
	//参数设置
	int nBg = 200;		//用来建立背景模型的数量
	float wVar = 1;		//方差权重

	int cnt = 0;
	cv::Mat frame;
	cv::Mat meanMat;
	cv::Mat varMat;
	cv::Mat dstMat;

	while (true)
	{
		capVideo >> frame;
		cvtColor(frame, frame, COLOR_BGR2GRAY);

		//前面的nBg帧，计算背景
		if (cnt < nBg) {

			srcMats.push_back(frame);

			if (cnt == 0) {
				std::cout << "reading frame " << std::endl;
			}

		}
		else if (cnt == nBg) {
			//计算模型
			meanMat.create(frame.size(),CV_8UC1);
			varMat.create(frame.size(),CV_32FC1);
			std::cout << "calculating background models" << std::endl;
			calcGaussianBackground(srcMats,meanMat,varMat);
		}
		else {
			//背景差分
			dstMat.create(frame.size(), CV_8UC1);
			gaussianThreshold(frame, meanMat, varMat, wVar, dstMat);
			imshow("result",dstMat);
			imshow("frame",frame);
			waitKey(30);
		}

		cnt++;

	}

	return 0;
}

int calcGaussianBackground(std::vector<cv::Mat> srcMats, cv::Mat & meanMat, cv::Mat &varMat)
{

	int rows = srcMats[0].rows;
	int cols = srcMats[0].cols;


	for (int h = 0; h < rows; h++)
	{
		for (int w = 0; w < cols; w++)
		{

			int sum=0;
			float var=0;
			//求均值
			for (int i = 0; i < srcMats.size(); i++) {
				sum += srcMats[i].at<uchar>(h, w);
			}
			meanMat.at<uchar>(h, w)=sum / srcMats.size();
			//求方差
			for (int i = 0; i < srcMats.size(); i++) {
				var += pow((srcMats[i].at<uchar>(h, w) - meanMat.at<uchar>(h, w)), 2);
			}
			varMat.at<float>(h, w) = var / srcMats.size();
		}
	}

	return 0;
 }

int gaussianThreshold(cv::Mat srcMat, cv::Mat meanMat, cv::Mat varMat, float weight, cv::Mat & dstMat)
{
	int srcI;
	int meanI;
	int dstI;
	int rows = srcMat.rows;
	int cols = srcMat.cols;

	for (int h = 0; h < rows; h++)
	{
		for (int w = 0; w < cols; w++)
		{
			srcI = srcMat.at<uchar>(h, w);
			meanI = meanMat.at<uchar>(h, w);
			int dif = abs(srcI - meanI);
			int th = weight*varMat.at<float>(h, w);

			if (dif > th) {

				dstMat.at<uchar>(h, w) = 255;
			}
			else {
				dstMat.at<uchar>(h, w)=0;
			}
		}
	}

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

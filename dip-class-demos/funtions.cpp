#include "stdafx.h"
#include "funtions.h"




using namespace cv;

//手动实现，通过 HOG (Histogram-of-Oriented-Gradients)比较图像相似度
int compareImages()
{

	//读入图像
	cv::Mat refMat	= imread("../testImages\\hogTemplate.jpg", 0);
	cv::Mat plMat	= imread("../testImages\\img1.jpg", 0);
	cv::Mat bgMat	= imread("../testImages\\img2.jpg", 0);

	if (refMat.empty() || plMat.empty() || bgMat.empty()) {
		std::cout << "failed to read image!:" << std::endl;
		return -1;
	}

	//参数设置
	int blockSize = 16;
	int nAngle = 8;
	int nX = refMat.cols / blockSize;
	int nY = refMat.rows / blockSize;

	int bins = nX*nY*nAngle;
	float * histogram = new float[bins];
	memset(histogram,0,sizeof(float)*bins);

	for (int i = 0; i < nY; i++) {
		for (int j = 0; j < nX; j++) {
			cv::Rect roi;
			cv::Mat roiMat;
			roi.x = j*blockSize;
			roi.y = i*blockSize;
			roi.width = blockSize;
			roi.height = blockSize;

			//赋值图像
			roiMat = refMat(roi);

			/*imshow("refMat", refMat);
			imshow("roiMat", roiMat);
			waitKey(0);*/
			//计算直方图
			calcHOG(roiMat,&histogram[i*j*nAngle],nAngle);

		}
	}

	delete[] histogram;

	return 0;
}

//手动实现 HOG (Histogram-of-Oriented-Gradients) 
int calcHOG(cv::Mat src, float * hist,int nAngle)
{

	Mat gx, gy;
	Mat mag, angle;
	Sobel(src, gx, CV_32F, 1, 0, 1);
	Sobel(src, gy, CV_32F, 0, 1, 1);

	//计算梯度及角度
	// x方向梯度，y方向梯度，梯度，角度，决定输出弧度or角度
	// 默认是弧度radians，可以选择角度degrees.
	cartToPolar(gx,gy,mag,angle,true);

	int binAngle = 360 / nAngle;

	for (int i = 0; i < src.rows; i++) {
		for (int j = 0; j < src.cols; j++) {
			//计算梯度
			int pos = angle.at<float>(i, j) / binAngle;
			hist[pos]+= mag.at<float>(i, j);
		}
	}


	return 0;
}

int calcHistDist(float * Hist1, float * Hist2, int size)
{
	return 0;
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


		std::vector<cv::Rect>::const_iterator it = found.begin();
		std::cout << "found:" << found.size() << std::endl;
		for (; it != found.end(); ++it) {
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

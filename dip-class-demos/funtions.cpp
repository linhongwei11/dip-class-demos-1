#include "stdafx.h"
#include "funtions.h"


//该demo验证并演示，视频中的像素灰度值变换是否呈高斯分布
int verifyGaussian()
{
	//----------------------读取视频文件--------------------------
	VideoCapture capVideo("../testImages\\vtest.avi");

	//如果视频打开失败
	if (!capVideo.isOpened()) {
		std::cout << "Unable to open video!" << std::endl;
		return -1;
	}








	return 0;
}

// dip-class-demos.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv.hpp>
#include "funtions.h"
#include "parameters.h"

using namespace cv;
using namespace std;

int main()
{
	//��ʼ��ʱ
	double start = static_cast<double>(cvGetTickCount());

	int demo_method = DEMO_METHOD;

	if (demo_method == 0) {
		//��demo��֤����ʾ����Ƶ�е����ػҶ�ֵ�任�Ƿ�ʸ�˹�ֲ�
		verifyGaussian();
	}
	else if(demo_method == 1) {
		//ʵ����ͨ�������
		bgSub_demo();
	}
	else if(demo_method == 2) {
		//ʵ�ָ�˹��ģ�ı������
		bgSubGaussian_demo();

	}
	else if (demo_method == 3) {
		//����opencv�ı�����ַ�demo
		opencvBgSubtrator();
	}

	//������ʱ
	double time = ((double)cvGetTickCount() - start) / cvGetTickFrequency();
	//��ʾʱ��
	cout << "processing time:" << time / 1000 << "ms" << endl;

	//�ȴ�������Ӧ�����������������
	system("pause");
    return 0;
}


// dip-class-demos.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <opencv.hpp>
#include "funtions.h"

using namespace cv;
using namespace std;

int main()
{
	//��ʼ��ʱ
	double start = static_cast<double>(cvGetTickCount());

	//����ͼ�εĻ���

	//����ֱ��
	//drawLines();

	//���ƾ���
	//drawRectangles();

	//����Բ��
	//drawCircles();

	//������Բ����Բ��
	//drawEllipse();

	//���Ʊ��
	//drawMarkers();

	//�ַ�
	writeText();

	//������ʱ
	double time = ((double)cvGetTickCount() - start) / cvGetTickFrequency();
	//��ʾʱ��
	cout << "processing time:" << time / 1000 << "ms" << endl;

	//�ȴ�������Ӧ�����������������
	system("pause");
    return 0;
}


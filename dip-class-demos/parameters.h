#pragma once


//ѡ����Ҫ��ʾ��demo
#define DEMO_METHOD		3	//0:��֤��Ƶ������ֵ�仯�Ƿ���ϸ�˹�ֲ�
							//1:��һ֡ͼ���������ļ򵥱������
							//2:��һ��˹�ֲ���ģ�������ʵ��
							//3:����opencv�ı�����ֺ���������ʾ


//��������
#define USE_CAMERA		true;	//ture: ʹ������ͷ��Ϊ����	falese����ȡ������Ƶ
#define VIDEO_PATH		"../testImages\\vtest.avi";	//�����ȡ������Ƶ����ʹ�ø�·��

//��˹��ģdemo����
#define FRAME_NUMBER	500	//ʹ����Ƶ��ǰ����֡���б�����ģ
#define VAR_WEIGHT		3	//������ֵʱ��������Ը�Ȩ��

//OpenCV������ֺ�������
#define BG_METHOD		0	//0��KNN 1��MOG2
#define UPDATE_MODEL	ture
#define SMOOTH_MASK		false


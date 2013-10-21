#pragma once
#include <cv.h>
#include <highgui.h>
#include <vector>
using namespace cv;

class EventNode
{
public:
	EventNode(void);
	~EventNode(void);
public:
	bool mark;
	int startFrame;
	int endFrame;
	int type; //���ͣ��ж��Ƿ���ѳ����ģ�0��ʾһ����ο�1��ʾ���ѳ����ģ�һֻ֡��һ�Σ�
	Rect rect;
	vector<Rect> trackList;
	//VideoCapture capture; 
};


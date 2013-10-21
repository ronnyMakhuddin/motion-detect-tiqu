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
	int type; //类型，判断是否分裂出来的，0表示一般矩形框，1表示分裂出来的（一帧只有一次）
	Rect rect;
	vector<Rect> trackList;
	//VideoCapture capture; 
};


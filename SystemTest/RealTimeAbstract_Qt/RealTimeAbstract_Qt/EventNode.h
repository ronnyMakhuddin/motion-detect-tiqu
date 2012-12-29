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
	Rect rect;
	int histomgram[13];
	vector<Rect> trackList;
	VideoCapture capture;
};


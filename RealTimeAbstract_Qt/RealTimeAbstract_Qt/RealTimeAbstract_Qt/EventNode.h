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
	vector<Rect> trackList;
};


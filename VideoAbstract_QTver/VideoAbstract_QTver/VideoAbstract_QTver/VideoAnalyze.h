#pragma once
#include <qstring.h>
#include <vector>
#include <qmessagebox.h>
#include <qthread.h>
#include "EventNode.h"
#include "EventNodeOperation.h"
class VideoAnalyze:public QThread
{
	Q_OBJECT
public:
	QString filePath;
	vector<EventNode> eventList;
	int fps;
	int minArea;
	int maxArea;
	int jiange;
	int key_jiange;
	int maxEventNum;
	int minEventNum;
	int LIMIT;
public:
	void analyzeVideo();  //∑÷Œˆ ”∆µ
	void update_mhi(IplImage*&img, IplImage*&dst, int frameNum, IplImage**&buf, int&last, IplImage*&mhi, CvSize size, double&lastTime);
	VideoAnalyze(void);
	virtual void run();
	VideoAnalyze(QObject* parent);
	~VideoAnalyze(void);
};


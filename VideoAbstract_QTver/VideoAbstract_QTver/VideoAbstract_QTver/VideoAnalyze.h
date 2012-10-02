#pragma once
#include <qstring.h>
#include <vector>
#include <qmessagebox.h>
#include <QtGui/QWidget>
#include <QFileDialog>
#include "EventNode.h"
#include "EventNodeOperation.h"
class VideoAnalyze
{
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
	VideoAnalyze(void);
	VideoAnalyze(QString filePath);
	~VideoAnalyze(void);
};


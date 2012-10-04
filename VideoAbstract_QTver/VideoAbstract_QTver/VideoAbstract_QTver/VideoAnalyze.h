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
	int frameCount;
	int key_jiange;
	int maxEventNum;
	int minEventNum;
	int LIMIT;
	bool isContinue;    //是否继续分析
	bool isShowVideo;   //是否显示视频
private:
	CvCapture *capture;
    IplImage *iplImg;
    IplImage *frame;
    QImage *qImg;
public:
	void analyzeVideo();  //分析视频
	void update_mhi(IplImage*&img, IplImage*&dst, int frameNum, IplImage**&buf, int&last, IplImage*&mhi, CvSize size, double&lastTime);
	VideoAnalyze(void);
	VideoAnalyze(QObject* parent);
	~VideoAnalyze(void);
protected:
	void run();
signals:
	void sendQImage(QImage, int); //显示图像
	void sendProcessBarValue(int);//更新进度条
	void sendOpenFileFailed();    //文件打开失败
};


#pragma once
#include <qstring.h>
#include <vector>
#include <qmessagebox.h>
#include <qthread.h>
#include <qdir.h>
#include "EventNode.h"
#include "EventNodeOperation.h"
#include "Globals.h"
#include "FileOperation.h"

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
	bool isSaveToFile;  //是否保存摘要去文件
private:
	CvCapture *capture;
    IplImage *iplImg;
    IplImage *frame;
    QImage *qImg;
public:
	void analyzeVideo();  //分析视频
	void update_mhi(IplImage*&img, IplImage*&dst, int frameNum, IplImage**&buf, int&last, IplImage*&mhi, CvSize size, double&lastTime);
	void getKeyFrameJiange();   //获取关键帧的间隔
	void saveEventToFile();     //将事件保存至文件
	void drawAbstracts();       //显示摘要事件列表
	VideoAnalyze(void);
	VideoAnalyze(QObject* parent);
	~VideoAnalyze(void);
protected:
	void run();
signals:
	void sendQImage(QImage, int); //显示图像
	void sendProcessBarValue(int);//更新进度条
	void sendOpenFileFailed();    //文件打开失败
	void sendDrawAbstracts(QImage,QString,QString);     //发送画摘要缩略图的信号
};


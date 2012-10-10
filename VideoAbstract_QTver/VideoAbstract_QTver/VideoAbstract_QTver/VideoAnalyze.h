#pragma once
#include <qstring.h>
#include <vector>
#include <qthread.h>
#include <qdir.h>
#include <qtime>
#include "EventNode.h"
#include "EventNodeOperation.h"
#include "Globals.h"
#include "FileOperation.h"

class VideoAnalyze:public QThread
{
	Q_OBJECT
public:
	QString filePath;
	QStringList filePathList;
	vector<EventNode> eventList;
	int fps;
	int minArea;
	int maxArea;
	int jiange;
	int frameCount;
	int key_jiange;
	int maxEventNum;
	int minEventNum;
	int LIMIT;          //同一时间最大事件数量
	bool isContinue;    //是否继续分析
	bool isShowVideo;   //是否显示视频
	bool isSaveToFile;  //是否保存摘要去文件
	bool isReadFromFile; //是否从文件读取摘要信息
	bool isBatch;       //是否是批量视频处理
	bool isIgnoreExistAnalyze;  //是否忽略已经分析过的视频
	CvSize captureSize;    //视频分辨率大小
private:
	CvCapture *capture;
	CvVideoWriter* videoWriter;
    IplImage *iplImg;
    IplImage *frame;
	IplImage *baseFrame;
    QImage *qImg;
public:
	void analyzeVideo();   //视频分析
	void singleAnalysis(); //单个视频分析
	void batchAnalysis();  //批量视频分析
	void update_mhi(IplImage*&img, IplImage*&dst, int frameNum, IplImage**&buf, int&last, IplImage*&mhi, CvSize size, double&lastTime);
	void getKeyFrameJiange();   //获取关键帧的间隔
	void saveEventToFile();     //将事件保存至文件
	void drawAbstracts();       //显示摘要事件列表
	void createAllEventVideo();  //创建所有摘要事件的视频
	void getBaseFrame();         //获得视频的关键帧
	bool init();                 //初始化一些关键变量
	void release();            //初始化一些Image
	VideoAnalyze(void);
	VideoAnalyze(QObject* parent);
	~VideoAnalyze(void);
protected:
	void run();
signals:
	void sendQImage(QImage, int); //显示图像
	void sendProcessBarValue(int);//更新进度条
	void sendOpenFileFailed();    //文件打开失败
	void sendDrawAbstracts(QImage,QString,QString,int);     //发送画摘要缩略图的信号
	void sendProcessInfo(QString);                      //发送视频处理信息
};


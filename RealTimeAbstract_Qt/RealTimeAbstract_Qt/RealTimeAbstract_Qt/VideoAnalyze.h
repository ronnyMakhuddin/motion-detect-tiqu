#pragma once
#include <qstring.h>
#include <vector>
#include <qthread.h>
#include <qdir.h>
#include <qtime>
#include <QTimer>
#include "EventNode.h"
#include "EventNodeOperation.h"
#include "Globals.h"
#include "FileOperation.h"
#include "setting_widget.h"

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
	bool isRealTime;    //是否实时分析
	bool isIgnoreExistAnalyze;  //是否忽略已经分析过的视频
	CvSize captureSize;    //视频分辨率大小

	int runSeconds;     //运行时间

	int flag;
	static int SELECTEVENT;

	Point lineP1;  //事件筛选的4个点（一条直线和一个矩形）
	Point lineP2;
	Point rectP1;
	Point rectP2;
	QString data;  //事件筛选的条件数据
private:
	CvCapture *capture;
	CvVideoWriter* videoWriter;
    IplImage *iplImg;
    IplImage *frame;
	IplImage *baseFrame;
    QImage *qImg;
	QTimer *timer;
public:
	void analyzeVideo();   //视频分析
	void analyzeRealTimeVideo(); //分析实时视频
	void singleAnalysis(); //单个视频分析
	void batchAnalysis();  //批量视频分析
	void realTimeAnalysis();  //实时视频分析
	void videoSearch();       //视频检索
	void typeFilter(int type); //类别过滤器
	void colorFilter(int r, int g, int b); //颜色过滤器
	bool isHumanEvent(EventNode node, CascadeClassifier detector);       //判断该事件是不是人物事件
	bool isCarEvent(EventNode node, CascadeClassifier detector);    //判断该事件是不是车辆事件
	void update_mhi(IplImage*&img, IplImage*&dst, int frameNum, IplImage**&buf, int&last, IplImage*&mhi, CvSize size, double&lastTime);
	void getKeyFrameJiange();   //获取关键帧的间隔
	void saveEventToFile();     //将事件保存至文件
	void drawAbstracts();       //显示摘要事件列表
	void createAllEventVideo();  //创建所有摘要事件的视频
	void getBaseFrame();         //获得视频的关键帧
	bool init();                 //初始化一些关键变量
	void release();            //初始化一些Image
	bool initRealTime();         //初始化实时视频
	IplImage* getFrameByNumber(int);  //通过位置获取帧
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
	void sendChangeAnalyzeButtonText(QString);  //改变分析按钮的文字
	void sendAnalyzeButtonState(int);            //改变分析按钮状态
	void sendQImage2(QImage);
	void sendRunTime(QString);   //发送运行时间
	void sendEndTimeCount();     //发送结束计时新号
	void sendEventCount(int num); //发送事件数量
	void sendRemoveAllAbstracts(); //通知界面删除所有摘要事件
private slots:
	void updateOneSecond();       //更新时间
	void getSettingData(int,int,int,int,int,int,int,int); //获取设置信息
	void getShuaixuanData(Point,Point,Point,Point,QString);    //获取筛选信息
};


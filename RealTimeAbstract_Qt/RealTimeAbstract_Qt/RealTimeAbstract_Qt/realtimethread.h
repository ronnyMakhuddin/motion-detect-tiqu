#ifndef REALTIMETHREAD_H
#define REALTIMETHREAD_H

#include <QThread>
#include <QImage>
#include <QTimer>
#include <QDateTime>
#include <QStringList>
#include <QDir>
#include <cv.h>
#include <highgui.h>
#include "EventNode.h"
#include "EventNodeOperation.h"
#include "FileOperation.h"


class RealTimeThread : public QThread
{
	Q_OBJECT

public:
	bool init(); //初始化
	void release(); //释放内存
	void setNum(int num); //设置摄像头编号
	void startMonitor();  //开始监控
	void endMonitor();    //停止监控
	void saveToFile();    //保存视频到文件
	void initShowFPS(); //初始化显示帧率模块
	void showFPS();     //显示帧率
	void releseShowFPS(); //释放显示帧率模块
	void setDataFromSetting(QString data);  //从设置界面设置分析参数
	void saveEventToFile();     //将事件保存至文件
	RealTimeThread(QObject *parent);
	~RealTimeThread();

private:
	void update_mhi(IplImage*&img, IplImage*&dst, int frameNum, IplImage**&buf, int&last, IplImage*&mhi, CvSize size, double&lastTime);

protected:
	void run();

public:
	int flag;

private:
	//opencv图像变量
	int num;     //摄像头编号
    bool isShowFPS;  //是否显示帧率
	bool isSaveToFile; //是否保存到本地文件
	int realTimeFps; //实时帧率
	int frameCountInSecond; //一秒钟的帧数计数器

	int scale;   //视频缩放
	int fps;     //录制帧率
	int min_area;   //最小面积
	int max_area;  //最大面积
	int jiange;    //分析帧间隔
	int max_event_num; //同一时间最大摘要数
	QString fileDir;  //保存的文件夹路径
	QString fileName;  //文件名字

	CvCapture* capture;   //视频
	CvVideoWriter* writer; //写视频
	IplImage* iplImg;     //图像，和QImage绑定
    IplImage* frame;      //视频帧
    QImage* qImg;
	CvSize captureSize;   //视频大小
	CvFont fontFPS;	      //帧率的显示字体
	//其它变量
	vector<EventNode> eventList;
signals:
	void sendCameraImage(int, QImage);
private slots:
	void timerTimeOut();  //计时器到时（1s）
};

#endif // ANALYZETHREAD_H

#ifndef REALTIMETHREAD_H
#define REALTIMETHREAD_H

#include <QThread>
#include <QImage>
#include <QTimer>
#include <QDateTime>
#include <cv.h>
#include <highgui.h>

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
	RealTimeThread(QObject *parent);
	~RealTimeThread();

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
	CvCapture* capture;   //视频
	CvVideoWriter* writer; //写视频
	IplImage* iplImg;     //图像，和QImage绑定
    IplImage* frame;      //视频帧
    QImage* qImg;
	CvSize captureSize;   //视频大小
	CvFont fontFPS;	      //帧率的显示字体
	//其它变量
	QString fileDir;      //保存的文件的目录字符串

signals:
	void sendCameraImage(int, QImage);
private slots:
	void timerTimeOut();  //计时器到时（1s）
};

#endif // ANALYZETHREAD_H

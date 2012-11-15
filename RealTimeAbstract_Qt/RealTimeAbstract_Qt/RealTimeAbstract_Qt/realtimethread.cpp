#include "realtimethread.h"

//public functions
//线程类初始化函数
bool RealTimeThread::init()
{
	capture = cvCaptureFromCAM(num);
	if(!capture)
		return false;
	captureSize = cvSize((int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
			(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
	qImg = new QImage(QSize(captureSize.width,captureSize.height), QImage::Format_RGB888);
	iplImg = cvCreateImageHeader(captureSize,  8, 3);
	iplImg->imageData = (char*)qImg->bits();
	return true;
}

//线程类释放函数
void RealTimeThread::release()
{
	//msleep(100);
	while(this->isRunning())
	{
		msleep(10);
	}
	if(capture)
		cvReleaseCapture(&capture);
	if(writer)
		cvReleaseVideoWriter(&writer);
	if(iplImg)
		cvReleaseImage(&iplImg);
}

//设置摄像头编号
void RealTimeThread::setNum(int num)
{
	this->num = num;
}

//开始监控
void RealTimeThread::startMonitor()
{
	//获取本地时间，作为视频文件名
	QString time = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_");
	QString filePath = fileDir + time + QString::number(num+1) + tr(".avi");
	QByteArray ba = filePath.toLocal8Bit();
	char* filePath_c = ba.data();
	//注意字符串处理，视频文件名和路径名不能有中文，否则会出错
	writer = cvCreateVideoWriter(filePath_c, CV_FOURCC('X', 'V', 'I', 'D'), 25, captureSize, 1);
	isSaveToFile = true;
}

//停止监控
void RealTimeThread::endMonitor()
{
	isSaveToFile = false;
	msleep(100);
	cvReleaseVideoWriter(&writer);
	writer = 0;
}

//保存视频到文件
void RealTimeThread::saveToFile()
{
	if(!isSaveToFile || !writer)
		return;
	cvWriteFrame(writer, frame);
}

//初始化显示帧率模块
void RealTimeThread::initShowFPS()
{
	realTimeFps = 0;
	frameCountInSecond = 0;
	cvInitFont(&fontFPS, CV_FONT_HERSHEY_SIMPLEX, 1, 1, 1, 1, CV_AA);
}

//显示帧率
void RealTimeThread::showFPS()
{
	if(!isShowFPS)
		return;
	char eventNumber[128];
	sprintf(eventNumber, "fps=%d", realTimeFps);
	cvPutText(frame, eventNumber, cvPoint(10, 30), &fontFPS, cvScalar(255,0,0));
	frameCountInSecond++;
}

//释放显示帧率模块
void RealTimeThread::releseShowFPS()
{
}

void RealTimeThread::setDataFromSetting(QString data)
{
	QStringList dataList = data.split('|');
	scale = dataList.at(0).toInt();   //视频缩放
	min_area = dataList.at(1).toInt();   //最小面积
	max_area = dataList.at(2).toInt();  //最大面积
	jiange = dataList.at(3).toInt();    //分析帧间隔
	fps = dataList.at(4).toInt();     //录制帧率
	max_event_num = dataList.at(5).toInt(); //同一时间最大摘要数
	fileDir = dataList.at(6);  //保存的文件夹路径
}

//构造函数
RealTimeThread::RealTimeThread(QObject *parent)
	: QThread(parent)
{
	capture = 0;
	writer = 0;
	iplImg = 0;
    frame = 0;
    qImg = 0;
	fileDir = QString("D:\\realTimeVideo\\");
	isShowFPS = true;
	isSaveToFile = false;
	if(isShowFPS)
		initShowFPS();  //初始化显示帧率
}

RealTimeThread::~RealTimeThread()
{

}

//protected functions
void RealTimeThread::run()
{	
	init();
	
    
	flag = 1;
	int frameNum = 0;
	for(; flag; frameNum++)
	{
		frame = cvQueryFrame(capture);  //获取帧
		saveToFile();    //保存文件
		showFPS();       //显示帧率
		if (frame->origin == IPL_ORIGIN_TL)  
		{  
			cvCopy(frame,iplImg,0);  
		}  
		else  
		{  
			cvFlip(frame,iplImg,0);  
		}
		cvCvtColor(iplImg,iplImg,CV_BGR2RGB);
		emit sendCameraImage(num, *qImg);
		//msleep(20);
	}
}

//时钟到点的slot函数
void RealTimeThread::timerTimeOut()
{
	realTimeFps = frameCountInSecond;
	frameCountInSecond = 0;
}
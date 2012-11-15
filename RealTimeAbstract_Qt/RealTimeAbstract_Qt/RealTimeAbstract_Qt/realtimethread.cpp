#include "realtimethread.h"

//public functions
//�߳����ʼ������
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

//�߳����ͷź���
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

//��������ͷ���
void RealTimeThread::setNum(int num)
{
	this->num = num;
}

//��ʼ���
void RealTimeThread::startMonitor()
{
	//��ȡ����ʱ�䣬��Ϊ��Ƶ�ļ���
	QString time = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_");
	QString filePath = fileDir + time + QString::number(num+1) + tr(".avi");
	QByteArray ba = filePath.toLocal8Bit();
	char* filePath_c = ba.data();
	//ע���ַ���������Ƶ�ļ�����·�������������ģ���������
	writer = cvCreateVideoWriter(filePath_c, CV_FOURCC('X', 'V', 'I', 'D'), 25, captureSize, 1);
	isSaveToFile = true;
}

//ֹͣ���
void RealTimeThread::endMonitor()
{
	isSaveToFile = false;
	msleep(100);
	cvReleaseVideoWriter(&writer);
	writer = 0;
}

//������Ƶ���ļ�
void RealTimeThread::saveToFile()
{
	if(!isSaveToFile || !writer)
	{
		return;
	}
	cvWriteFrame(writer, frame);
}

//��ʼ����ʾ֡��ģ��
void RealTimeThread::initShowFPS()
{
	realTimeFps = 0;
	frameCountInSecond = 0;
	cvInitFont(&fontFPS, CV_FONT_HERSHEY_SIMPLEX, 1, 1, 1, 1, CV_AA);
}

//��ʾ֡��
void RealTimeThread::showFPS()
{
	if(!isShowFPS)
		return;
	char eventNumber[128];
	sprintf(eventNumber, "fps=%d", realTimeFps);
	cvPutText(frame, eventNumber, cvPoint(10, 30), &fontFPS, cvScalar(255,0,0));
	frameCountInSecond++;
}

//�ͷ���ʾ֡��ģ��
void RealTimeThread::releseShowFPS()
{
}

void RealTimeThread::setDataFromSetting(QString data)
{
	QStringList dataList = data.split('|');
	scale = dataList.at(0).toInt();   //��Ƶ����
	min_area = dataList.at(1).toInt();   //��С���
	max_area = dataList.at(2).toInt();  //������
	jiange = dataList.at(3).toInt();    //����֡���
	fps = dataList.at(4).toInt();     //¼��֡��
	max_event_num = dataList.at(5).toInt(); //ͬһʱ�����ժҪ��
	fileDir = dataList.at(6);  //������ļ���·��
}

//���캯��
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
		initShowFPS();  //��ʼ����ʾ֡��
}

RealTimeThread::~RealTimeThread()
{

}

//protected functions
void RealTimeThread::run()
{	
	init();
	//�㷨��ʼ
	int N = 3;
	IplImage*motion = 0;   //�ڴ�δ�ͷţ�
	IplImage**buf = 0;
	IplImage*mhi = 0;
	int last = 0;
	double lastTime = 0;
	flag = 1;
	int frameNum = 0;

	mhi = cvCreateImage(captureSize, IPL_DEPTH_32F, 1);
	cvZero(mhi);
	buf = new IplImage*[N];
	for (int i = 0; i < N; i++)
	{
		//cvReleaseImage(&buf[i]);
		buf[i] = cvCreateImage(captureSize, IPL_DEPTH_8U, 1);
		cvZero(buf[i]);
	}
	for(; flag; )
	{
		frame = cvQueryFrame(capture);  //��ȡ֡

		//�㷨����
		if (!frame || !flag)
		{
			msleep(100);//�����������ͣ�Ļ�������Ϊ��ʵͼƬ���ͷŶ������ڴ����
			break;
		}

		if(isSaveToFile)
		{
			saveToFile();    //�����ļ�
		}


		showFPS();       //��ʾ֡��
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

//ʱ�ӵ����slot����
void RealTimeThread::timerTimeOut()
{
	realTimeFps = frameCountInSecond;
	frameCountInSecond = 0;
}
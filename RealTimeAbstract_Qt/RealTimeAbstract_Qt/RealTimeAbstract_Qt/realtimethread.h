#ifndef REALTIMETHREAD_H
#define REALTIMETHREAD_H

#include <QThread>
#include <QImage>
#include <QTimer>
#include <QDateTime>
#include <QStringList>
#include <cv.h>
#include <highgui.h>
#include "EventNode.h"
#include "EventNodeOperation.h"

class RealTimeThread : public QThread
{
	Q_OBJECT

public:
	bool init(); //��ʼ��
	void release(); //�ͷ��ڴ�
	void setNum(int num); //��������ͷ���
	void startMonitor();  //��ʼ���
	void endMonitor();    //ֹͣ���
	void saveToFile();    //������Ƶ���ļ�
	void initShowFPS(); //��ʼ����ʾ֡��ģ��
	void showFPS();     //��ʾ֡��
	void releseShowFPS(); //�ͷ���ʾ֡��ģ��
	void setDataFromSetting(QString data);  //�����ý������÷�������
	RealTimeThread(QObject *parent);
	~RealTimeThread();

protected:
	void run();

public:
	int flag;

private:
	//opencvͼ�����
	int num;     //����ͷ���
    bool isShowFPS;  //�Ƿ���ʾ֡��
	bool isSaveToFile; //�Ƿ񱣴浽�����ļ�
	int realTimeFps; //ʵʱ֡��
	int frameCountInSecond; //һ���ӵ�֡��������

	int scale;   //��Ƶ����
	int fps;     //¼��֡��
	int min_area;   //��С���
	int max_area;  //������
	int jiange;    //����֡���
	int max_event_num; //ͬһʱ�����ժҪ��
	QString fileDir;  //������ļ���·��

	CvCapture* capture;   //��Ƶ
	CvVideoWriter* writer; //д��Ƶ
	IplImage* iplImg;     //ͼ�񣬺�QImage��
    IplImage* frame;      //��Ƶ֡
    QImage* qImg;
	CvSize captureSize;   //��Ƶ��С
	CvFont fontFPS;	      //֡�ʵ���ʾ����
	//��������

signals:
	void sendCameraImage(int, QImage);
private slots:
	void timerTimeOut();  //��ʱ����ʱ��1s��
};

#endif // ANALYZETHREAD_H

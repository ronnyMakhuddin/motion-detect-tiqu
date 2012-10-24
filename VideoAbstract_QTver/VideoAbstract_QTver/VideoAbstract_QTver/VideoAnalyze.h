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
	int LIMIT;          //ͬһʱ������¼�����
	bool isContinue;    //�Ƿ��������
	bool isShowVideo;   //�Ƿ���ʾ��Ƶ
	bool isSaveToFile;  //�Ƿ񱣴�ժҪȥ�ļ�
	bool isReadFromFile; //�Ƿ���ļ���ȡժҪ��Ϣ
	bool isBatch;       //�Ƿ���������Ƶ����
	bool isRealTime;    //�Ƿ�ʵʱ����
	bool isIgnoreExistAnalyze;  //�Ƿ�����Ѿ�����������Ƶ
	CvSize captureSize;    //��Ƶ�ֱ��ʴ�С

	int runSeconds;     //����ʱ��
private:
	CvCapture *capture;
	CvVideoWriter* videoWriter;
    IplImage *iplImg;
    IplImage *frame;
	IplImage *baseFrame;
    QImage *qImg;
	QTimer *timer;
public:
	void analyzeVideo();   //��Ƶ����
	void analyzeRealTimeVideo(); //����ʵʱ��Ƶ
	void singleAnalysis(); //������Ƶ����
	void batchAnalysis();  //������Ƶ����
	void realTimeAnalysis();  //ʵʱ��Ƶ����
	void update_mhi(IplImage*&img, IplImage*&dst, int frameNum, IplImage**&buf, int&last, IplImage*&mhi, CvSize size, double&lastTime);
	void getKeyFrameJiange();   //��ȡ�ؼ�֡�ļ��
	void saveEventToFile();     //���¼��������ļ�
	void drawAbstracts();       //��ʾժҪ�¼��б�
	void createAllEventVideo();  //��������ժҪ�¼�����Ƶ
	void getBaseFrame();         //�����Ƶ�Ĺؼ�֡
	bool init();                 //��ʼ��һЩ�ؼ�����
	void release();            //��ʼ��һЩImage
	bool initRealTime();         //��ʼ��ʵʱ��Ƶ
	VideoAnalyze(void);
	VideoAnalyze(QObject* parent);
	~VideoAnalyze(void);
protected:
	void run();
signals:
	void sendQImage(QImage, int); //��ʾͼ��
	void sendProcessBarValue(int);//���½�����
	void sendOpenFileFailed();    //�ļ���ʧ��
	void sendDrawAbstracts(QImage,QString,QString,int);     //���ͻ�ժҪ����ͼ���ź�
	void sendProcessInfo(QString);                      //������Ƶ������Ϣ
	void sendChangeAnalyzeButtonText(QString);
	void sendQImage2(QImage);
	void sendRunTime(QString);   //��������ʱ��
	void sendEndTimeCount();     //���ͽ�����ʱ�º�
	void sendEventCount(int num); //�����¼�����
private slots:
	void updateOneSecond();       //����ʱ��
};


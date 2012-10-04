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
	bool isContinue;    //�Ƿ��������
	bool isShowVideo;   //�Ƿ���ʾ��Ƶ
private:
	CvCapture *capture;
    IplImage *iplImg;
    IplImage *frame;
    QImage *qImg;
public:
	void analyzeVideo();  //������Ƶ
	void update_mhi(IplImage*&img, IplImage*&dst, int frameNum, IplImage**&buf, int&last, IplImage*&mhi, CvSize size, double&lastTime);
	VideoAnalyze(void);
	VideoAnalyze(QObject* parent);
	~VideoAnalyze(void);
protected:
	void run();
signals:
	void sendQImage(QImage, int); //��ʾͼ��
	void sendProcessBarValue(int);//���½�����
	void sendOpenFileFailed();    //�ļ���ʧ��
};


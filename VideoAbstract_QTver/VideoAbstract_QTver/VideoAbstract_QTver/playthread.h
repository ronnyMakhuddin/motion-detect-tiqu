#ifndef PLAYTHREAD_H
#define PLAYTHREAD_H

#include <QThread>
#include <cv.h>
#include <highgui.h>
#include <qimage.h>
#include "EventNode.h"

class PlayThread : public QThread
{
	Q_OBJECT

public:
	bool init(QString,EventNode,int);
	PlayThread(QObject *parent);
	~PlayThread();
signals:
	void sendPlayImage(QImage);
	void sendSliderRange(int,int);
	void sendSliderValue(int);
	void threadEnd();   //播放完毕后发送此消息，让按钮状态改变
protected:
	void run();
private:
	

public:
	QString filePath;
	CvCapture* capture;
	IplImage *iplImg;
    IplImage *frame;
    QImage *qImg;
	CvSize captureSize;
	int fps;
	int jiange;
	int frameCount;
	EventNode node;
	bool isPlaying;
	int pos;
};

#endif // PLAYTHREAD_H

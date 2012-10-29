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
};

#endif // PLAYTHREAD_H

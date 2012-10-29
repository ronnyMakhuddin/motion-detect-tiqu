#ifndef PLAYTHREAD_H
#define PLAYTHREAD_H

#include <QThread>
#include <cv.h>
#include <highgui.h>
#include <qimage.h>

class PlayThread : public QThread
{
	Q_OBJECT

public:
	bool init(QString,int,int,int);
	PlayThread(QObject *parent);
	~PlayThread();
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
	int startFrame;
	int endFrame;
	int frameCount;
};

#endif // PLAYTHREAD_H

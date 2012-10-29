#include "playthread.h"

PlayThread::PlayThread(QObject *parent=0)
	: QThread(parent)
{

}

PlayThread::~PlayThread()
{

}

void PlayThread::run()
{
	int trackIndex = 0;
	cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, node.startFrame);
	for(int i = node.startFrame; i < node.endFrame; i++)
	{
		frame = cvQueryFrame(capture);
		trackIndex = (i-node.startFrame) / jiange;
		Rect r = node.trackList[trackIndex];
		cvRectangle(frame, cvPoint(r.x, r.y), cvPoint(r.x+r.width, r.y+r.height), cvScalar(255, 0, 0));
		if (frame->origin == IPL_ORIGIN_TL)  
		{  
			cvCopy(frame,iplImg,0);  
		}  
		else  
		{  
			cvFlip(frame,iplImg,0);  
		}  
		cvCvtColor(iplImg,iplImg,CV_BGR2RGB);
		emit sendPlayImage(*qImg);
		msleep(20);
	}
}

bool PlayThread::init(QString filePath, EventNode node, int jiange)
{
	this->filePath = filePath;
	this->node = node;
	QByteArray ba = filePath.toLocal8Bit();
	const char *file = ba.data();
	capture = cvCaptureFromAVI(file);
	if(!capture)
		return false;
	this->jiange = jiange;
	captureSize = cvSize((int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
			(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
	fps = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	frameCount = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	qImg = new QImage(QSize(captureSize.width,captureSize.height), QImage::Format_RGB888);
	iplImg = cvCreateImageHeader(captureSize,  8, 3);
	iplImg->imageData = (char*)qImg->bits();
	return true;
}
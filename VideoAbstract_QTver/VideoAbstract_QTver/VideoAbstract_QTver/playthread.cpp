#include "playthread.h"

PlayThread::PlayThread(QObject *parent=0)
	: QThread(parent)
{
	isPlaying = false;
	pos = 0;
	isLoop = false;
}

PlayThread::~PlayThread()
{

}

void PlayThread::run()
{
	
	int trackIndex = 0;
	cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, pos);
	for(; isPlaying; pos++)
	{
		if(pos >= node.endFrame)
		{
			if(isLoop)
			{
				pos = node.startFrame;
				cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, pos);
			}else
			{
				break;
			}
		}
		frame = cvQueryFrame(capture);
		trackIndex = (pos-node.startFrame) / jiange;
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
		emit sendSliderValue(pos);
		msleep(20);
	}
	if(isPlaying)
	{
		emit threadEnd();
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
	emit sendSliderRange(node.startFrame, node.endFrame-1);
	return true;
}

void PlayThread::getFrameByPos(int value)
{
	pos = value;
	cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, pos);
	frame = cvQueryFrame(capture);
	int trackIndex = (pos-node.startFrame) / jiange;
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
}
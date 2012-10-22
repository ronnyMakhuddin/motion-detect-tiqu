#include "abstractplayer.h"

AbstractPlayer::AbstractPlayer()
	: QDialog()
{
	ui.setupUi(this);
}

AbstractPlayer::~AbstractPlayer()
{

}

void AbstractPlayer::init(QString filePath)
{
	this->filePath = filePath;
	QByteArray ba = filePath.toLocal8Bit();
	const char *file = ba.data();
	capture = cvCaptureFromAVI(file);
	if(!capture)
		return;
	captureSize = cvSize((int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
			(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
	fps = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	//frameCount = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	qImg = new QImage(QSize(captureSize.width,captureSize.height), QImage::Format_RGB888);
	iplImg = cvCreateImageHeader(captureSize,  8, 3);
	iplImg->imageData = (char*)qImg->bits();
}

void AbstractPlayer::on_play_button_clicked()
{
	while((frame = cvQueryFrame(capture)))
	{
		if (frame)  
		{  
			if (frame->origin == IPL_ORIGIN_TL)  
			{  
				cvCopy(frame,iplImg,0);  
			}  
			else  
			{  
				cvFlip(frame,iplImg,0);  
			}  
			cvCvtColor(iplImg,iplImg,CV_BGR2RGB);
			ui.image_label->setPixmap(QPixmap::fromImage(*qImg));
		}  
	}
}

void AbstractPlayer::showImage(QImage image)
{
	QImage newImg = image.scaled(ui.image_label->width(), ui.image_label->height());
	ui.image_label->setPixmap(QPixmap::fromImage(newImg));
}
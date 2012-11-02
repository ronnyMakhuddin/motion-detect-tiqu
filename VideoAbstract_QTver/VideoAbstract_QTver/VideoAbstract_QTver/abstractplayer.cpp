#include "abstractplayer.h"

AbstractPlayer::AbstractPlayer()
	: QDialog()
{
	ui.setupUi(this);
	thread = new PlayThread(this);
	connect(thread, SIGNAL(sendPlayImage(QImage)), this,SLOT(showImage(QImage)));
	connect(thread, SIGNAL(sendSliderRange(int,int)), this, SLOT(setSliderRange(int,int)));
	connect(thread, SIGNAL(sendSliderValue(int)), this, SLOT(setSliderValue(int)));

	connect(ui.play_slider, SIGNAL(valueChanged(int)), this, SLOT(getSliderMoved(int)));
	//sliderMoved ( int value )只有在选中并且移动的时候才触发
	//valueChanged ( int value )在播放的时候改变value也会触发，不用鼠标选中
}

AbstractPlayer::~AbstractPlayer()
{

}

void AbstractPlayer::init(QString filePath)
{
	thread = new PlayThread(this);
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
	if(!thread->isPlaying)
	{
		thread->isPlaying = true;
		ui.play_button->setText(tr("暂停"));
	}else
	{
		thread->isPlaying = false;
		ui.play_button->setText(tr("开始"));
	}
}

void AbstractPlayer::showImage(QImage image)
{
	QImage newImg = image.scaled(ui.image_label->width(), ui.image_label->height());
	ui.image_label->setPixmap(QPixmap::fromImage(newImg));
}

void AbstractPlayer::getSliderMoved(int value)
{
	value++;
	value--;
}

void AbstractPlayer::setSliderValue(int value)
{
	ui.play_slider->setValue(value);
}

void AbstractPlayer::setSliderRange(int min, int max)
{
	ui.play_slider->setRange(min, max);
}
#include "abstractplayer.h"

AbstractPlayer::AbstractPlayer()
	: QDialog()
{
	ui.setupUi(this);
	thread = new PlayThread(this);
	connect(thread, SIGNAL(sendPlayImage(QImage)), this,SLOT(showImage(QImage)));
	connect(thread, SIGNAL(sendSliderRange(int,int)), this, SLOT(setSliderRange(int,int)));
	connect(thread, SIGNAL(sendSliderValue(int)), this, SLOT(setSliderValue(int)));
	connect(thread, SIGNAL(threadEnd()), this, SLOT(getThreadEnd()));

	connect(ui.play_slider, SIGNAL(sliderMoved(int)), this, SLOT(getSliderMoved(int)));
	connect(ui.loop_checkbox, SIGNAL(stateChanged (int)), this, SLOT(getLoopCheckBoxStateChange(int)));
	//sliderMoved ( int value )ֻ����ѡ�в����ƶ���ʱ��Ŵ���
	//valueChanged ( int value )�ڲ��ŵ�ʱ��ı�valueҲ�ᴥ�����������ѡ��
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
		ui.play_button->setText(tr("��ͣ"));
		thread->start();
	}else
	{
		thread->isPlaying = false;
		ui.play_button->setText(tr("��ʼ"));
	}
}

void AbstractPlayer::on_exit_button_clicked()
{
	this->hide(); 
}

void AbstractPlayer::showImage(QImage image)
{
	QImage newImg = image.scaled(ui.image_label->width(), ui.image_label->height());
	ui.image_label->setPixmap(QPixmap::fromImage(newImg));
}

void AbstractPlayer::getSliderMoved(int value)
{
	thread->getFrameByPos(value);
}

void AbstractPlayer::setSliderValue(int value)
{
	ui.play_slider->setValue(value);
}

void AbstractPlayer::setSliderRange(int min, int max)
{
	ui.play_slider->setRange(min, max);
}

void AbstractPlayer::getThreadEnd()
{
	thread->isPlaying = false;
	ui.play_button->setText(tr("��ʼ"));
	//���ý�������pos
	ui.play_slider->setValue(thread->node.startFrame);
	thread->pos = thread->node.startFrame;
}

void AbstractPlayer::getLoopCheckBoxStateChange(int state)
{
	thread->isLoop = ui.loop_checkbox->isChecked();
}
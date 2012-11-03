#ifndef ABSTRACTPLAYER_H
#define ABSTRACTPLAYER_H

#include <QDialog>
#include <cv.h>
#include <highgui.h>
#include "ui_playform.h"
#include "playthread.h"
#include "drawform.h"

class AbstractPlayer : public QDialog
{
	Q_OBJECT

public:
	AbstractPlayer();
	~AbstractPlayer();

public:
	Ui::PlayForm ui;
	QString filePath;
	CvCapture* capture;
	IplImage *iplImg;
    IplImage *frame;
    QImage *qImg;
	CvSize captureSize;
	int fps;

	PlayThread*thread;
public:
	void init(QString filePath);
private slots:
	void on_play_button_clicked();
	void showImage(QImage image);
	void getSliderMoved(int);
	void setSliderValue(int value);
	void setSliderRange(int min, int max);
	void getThreadEnd();
};

#endif // ABSTRACTPLAYER_H

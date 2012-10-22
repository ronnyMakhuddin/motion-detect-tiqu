#ifndef ABSTRACTPLAYER_H
#define ABSTRACTPLAYER_H

#include <QDialog>
#include <cv.h>
#include <highgui.h>
#include "ui_playform.h"

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
public:
	void init(QString filePath);
private slots:
	void on_play_button_clicked();
};

#endif // ABSTRACTPLAYER_H

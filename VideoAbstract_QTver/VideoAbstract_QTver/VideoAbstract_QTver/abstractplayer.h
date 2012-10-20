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
	int 
public:
	void init(QString filePath);
};

#endif // ABSTRACTPLAYER_H

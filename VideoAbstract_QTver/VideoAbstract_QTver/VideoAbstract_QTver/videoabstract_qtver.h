#ifndef VIDEOABSTRACT_QTVER_H
#define VIDEOABSTRACT_QTVER_H

#include <QtGui/QWidget>
#include "ui_videoabstract_qtver.h"

class VideoAbstract_QTver : public QWidget
{
	Q_OBJECT

public:
	VideoAbstract_QTver(QWidget *parent = 0, Qt::WFlags flags = 0);
	~VideoAbstract_QTver();

protected:
	void resizeEvent(QResizeEvent*event);
private:
	Ui::VideoAbstract_QTverClass ui;
};

#endif // VIDEOABSTRACT_QTVER_H

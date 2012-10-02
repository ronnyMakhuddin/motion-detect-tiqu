#ifndef VIDEOABSTRACT_QTVER_H
#define VIDEOABSTRACT_QTVER_H

#include <QtGui/QWidget>
#include <QFileDialog>
#include <qmessagebox.h>
#include "ui_videoabstract_qtver.h"
#include "Globals.h"


class VideoAbstract_QTver : public QWidget
{
	Q_OBJECT

public:
	VideoAbstract_QTver(QWidget *parent = 0, Qt::WFlags flags = 0);
	~VideoAbstract_QTver();

protected:
	void resizeEvent(QResizeEvent*event);

private slots:
	void on_open_file_button_clicked();
	void on_analysis_button_clicked();

private:
	Ui::VideoAbstract_QTverClass ui;
};

#endif // VIDEOABSTRACT_QTVER_H

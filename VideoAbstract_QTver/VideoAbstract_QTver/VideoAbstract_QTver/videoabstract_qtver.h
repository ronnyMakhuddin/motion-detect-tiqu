#ifndef VIDEOABSTRACT_QTVER_H
#define VIDEOABSTRACT_QTVER_H

#include <QtGui/QWidget>
#include <QFileDialog>
#include <qmessagebox.h>
#include <QVBoxLayout>
#include "ui_videoabstract_qtver.h"
#include "Globals.h"
#include "VideoAnalyze.h"
#include "singleabstractlayout.h"
#include "setting_widget.h"


class VideoAbstract_QTver : public QWidget
{
	Q_OBJECT

public:
	void batchAnalysis();   //处理多个视频
	VideoAbstract_QTver(QWidget *parent = 0, Qt::WFlags flags = 0);
	~VideoAbstract_QTver();

protected:
	void resizeEvent(QResizeEvent*event);

private slots:
	void on_open_file_button_clicked();
	void on_analysis_button_clicked();
	void on_show_button_clicked();
	void on_end_button_clicked();
	void on_setting_button_clicked();
	void showVideo(QImage, int);
	void openFileFailed();
	void updateProcessBar(int);
	void drawAbstracts(QImage,QString,QString,int);
	void updateProcessInfo(QString);

public:
	VideoAnalyze *analyzeThread;
private:
	Ui::VideoAbstract_QTverClass ui;
	setting_widget*settingUI;
	QGridLayout*gLayout;

	int testInt;
};

#endif // VIDEOABSTRACT_QTVER_H

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
#include "mylabel.h"
#include "abstractform.h"
#include "abstractplayer.h"


class VideoAbstract_QTver : public QWidget
{
	Q_OBJECT

public:
	void batchAnalysis();   //处理多个视频
	void startTimeCount();  //开始计时
	VideoAbstract_QTver(QWidget *parent = 0, Qt::WFlags flags = 0);
	~VideoAbstract_QTver();

protected:
	void resizeEvent(QResizeEvent*event);

private slots:
	void on_open_file_button_clicked();
	void on_analysis_button_clicked();
	void on_setting_button_clicked();
	void on_search_button_clicked();
	void on_show_video_check_box_clicked();
	void showVideo(QImage, int);
	void openFileFailed();
	void updateProcessBar(int);
	void drawAbstracts(QImage,QString,QString,int);
	void updateProcessInfo(QString);
	void changeAnalyzeButton(QString);
	void updateRunTime();        //更新分析时间
	void endTimeCount();         //结束计时
	void updateEventCount(int);     //更新事件数量

public:
	VideoAnalyze *analyzeThread;
private:
	Ui::VideoAbstract_QTverClass ui;
	setting_widget*settingUI;
	QGridLayout*gLayout;
	AbstractPlayer* player;

	int testInt;

	QTimer*timer;
	int runSeconds;
	//MyLabel* myLabel;
};

#endif // VIDEOABSTRACT_QTVER_H

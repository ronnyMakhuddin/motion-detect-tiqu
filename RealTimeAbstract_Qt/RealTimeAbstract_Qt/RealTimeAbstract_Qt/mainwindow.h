#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QWidget>
#include <QImage>
#include <QDateTime>
#include <QFileDialog>
#include "ui_mainwindow.h"
#include "realtimethread.h"
#include "videoInput.h"
#include "realtimesetting.h"
#include "videoabstract_qtver.h"


class MainWindow : public QWidget
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0, Qt::WFlags flags = 0);
	~MainWindow();
private:
	void getCameraNum();  //获取usb摄像头数量
	void startCamThread(int); //开始摄像头线程
	void endCamThread(int);   //结束摄像头线程
private slots:
	//my slots
	void showVideo(int, QImage);//显示摄像头
	void updateTime();          //更新时间
	void updataAbstractNum(int,int);   //更新事件个数
	void switchToRealTimeForm();   //切换到实时视频界面

	//system slots
	void on_pushButton_start_clicked();
	void on_pushButton_setting_clicked();
	void on_pushButton_localModel_clicked();
	void on_checkBox_cam0_clicked();
	void on_checkBox_cam1_clicked();
	void on_checkBox_cam2_clicked();
	void on_checkBox_cam3_clicked();

private:
	Ui::MainWindowClass ui;

	VideoAbstract_QTver *localForm;

	RealTimeThread* rtThread[4];  //4条线程

	QTimer*timer;  //1秒钟计时器

	RealTimeSetting*realTimeSetting; //实时视频设置界面

	int num;
	int isStartMonitor;
};

#endif // MAINWINDOW_H

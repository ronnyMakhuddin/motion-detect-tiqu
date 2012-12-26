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
	void getCameraNum();  //��ȡusb����ͷ����
	void startCamThread(int); //��ʼ����ͷ�߳�
	void endCamThread(int);   //��������ͷ�߳�
private slots:
	//my slots
	void showVideo(int, QImage);//��ʾ����ͷ
	void updateTime();          //����ʱ��
	void updataAbstractNum(int,int);   //�����¼�����
	void switchToRealTimeForm();   //�л���ʵʱ��Ƶ����

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

	RealTimeThread* rtThread[4];  //4���߳�

	QTimer*timer;  //1���Ӽ�ʱ��

	RealTimeSetting*realTimeSetting; //ʵʱ��Ƶ���ý���

	int num;
	int isStartMonitor;
};

#endif // MAINWINDOW_H

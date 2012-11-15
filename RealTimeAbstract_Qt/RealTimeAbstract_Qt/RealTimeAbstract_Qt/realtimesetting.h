#ifndef REALTIMESETTING_H
#define REALTIMESETTING_H

#include <QDialog>
#include <QFileDialog>
#include "ui_realtimesetting.h"

class RealTimeSetting : public QDialog
{
	Q_OBJECT

public:
	QString getData();
	RealTimeSetting(QWidget *parent = 0);
	~RealTimeSetting();
private:
	void setToUi();
	void readFromUi();
	void setDefault();
private slots:
	void on_pushButton_cancel_clicked();   //ȡ����ť
	void on_pushButton_ok_clicked();       //ȷ����ť
	void on_pushButton_default_clicked();  //Ĭ�����ð�ť
	void on_pushButton_path_clicked();     //ѡ��·����ť
private:
	Ui::RealTimeSetting ui;

	int scale;
	int width;
	int height;
	int min_area;
	int max_area;
	int jiange;
	int fps;
	int max_event;
	QString filePath;
};

#endif // REALTIMESETTING_H

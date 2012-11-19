#include "mainwindow.h"

//public functions
MainWindow::MainWindow(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	ui.setupUi(this);
	setWindowFlags(windowFlags() &~ Qt::WindowMinMaxButtonsHint);
	setWindowFlags(windowFlags() &~ Qt::WindowCloseButtonHint);
	//setWindowFlags(windowFlags() &~ (Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint));
	setFixedSize(this->width(), this->height());

	for(int i = 0; i < 2; i++)
	{
		rtThread[i] = 0;
	}
	getCameraNum();  //获取摄像头数量
	//开始计时
	timer = new QTimer(0);
	timer->setInterval(1000);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
	timer->start();

	//设置界面
	realTimeSetting = new RealTimeSetting(0);

	isStartMonitor = false;
}

MainWindow::~MainWindow()
{
	while(rtThread[0] || rtThread[1])//释放掉2个线程
	{
		for(int i = 0; i < 2; i++)
		{
			if(!rtThread[i])
				continue;
			disconnect(rtThread[i], SIGNAL(sendCameraImage(int, QImage)), this, SLOT(showVideo(int, QImage)));
			disconnect(timer, SIGNAL(timeout()), rtThread[i], SLOT(timerTimeOut()));
			rtThread[i]->flag = 0;
			rtThread[i]->release();
			delete rtThread[i];
			rtThread[i] = 0;
		}
	}
}

//private functions:
void MainWindow::getCameraNum()  //获取摄像头数量，并且设置checkBox的可点击属性
{
	videoInput vi;
	num = vi.listDevices(true);
	ui.checkBox_cam0->setDisabled(false);
	ui.checkBox_cam1->setDisabled(false);
	switch(num)
	{
	case 0:
		ui.checkBox_cam0->setDisabled(true);
		ui.checkBox_cam1->setDisabled(true);
		break;
	case 1:
		ui.checkBox_cam1->setDisabled(true);
		break;
	case 2:
		break;
	}
}

void MainWindow::startCamThread(int index)
{
	rtThread[index] = new RealTimeThread(this);
	connect(rtThread[index], SIGNAL(sendCameraImage(int, QImage)), this, SLOT(showVideo(int, QImage)));
	connect(rtThread[index], SIGNAL(sendAbstractCount(int)), this, SLOT(updataAbstractNum(int)));
	connect(timer, SIGNAL(timeout()), rtThread[index], SLOT(timerTimeOut()));
	rtThread[index]->setNum(index);
	rtThread[index]->start();
}

void MainWindow::endCamThread(int index)
{
	disconnect(rtThread[index], SIGNAL(sendCameraImage(int, QImage)), this, SLOT(showVideo(int, QImage)));
	disconnect(timer, SIGNAL(timeout()), rtThread[index], SLOT(timerTimeOut()));
	rtThread[index]->flag = 0;
	rtThread[index]->release();
	delete rtThread[index];
	rtThread[index] = 0;
	
	switch(index)
	{
	case 0:
		ui.label_cam0->setPixmap(QPixmap(QString::fromUtf8(":/MainWindow/Resources/nocapture.png")));
		break;
	case 1:
		ui.label_cam1->setPixmap(QPixmap(QString::fromUtf8(":/MainWindow/Resources/nocapture.png")));
		break;
	}
}

//my slots
void MainWindow::showVideo(int num, QImage img)
{
	QImage image = img.scaled(ui.label_cam0->width(), ui.label_cam0->height());
	switch(num)
	{
	case 0:
		ui.label_cam0->setPixmap(QPixmap::fromImage(image));
		break;
	case 1:
		ui.label_cam1->setPixmap(QPixmap::fromImage(image));
		break;
	}
}

void MainWindow::updateTime()
{
	ui.label_time->setText(QDateTime::currentDateTime().toString(tr("时间:")+"yyyy-MM-dd hh:mm:ss ddd"));
}

void MainWindow::updataAbstractNum(int count)
{
	ui.label_abstract_num->setText(tr("人物数量:")+QString::number(count));
}

//system slots
void MainWindow::on_pushButton_start_clicked()
{
	for(int i = 0; i < 3; i++)//判断有没有打开摄像头
	{
		if(i == 2)
			return;
		if(rtThread[i])
			break;
	}

	if(!isStartMonitor)
	{
		for(int i = 0; i < num; i++)
		{
			if(rtThread[i])
			{
				rtThread[i]->setDataFromSetting(realTimeSetting->getData());
				rtThread[i]->startMonitor();
			}
		}
		isStartMonitor = true;
		QIcon icon;
        icon.addFile(QString::fromUtf8(":/MainWindow/Resources/stopanalyze.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui.pushButton_start->setIcon(icon);
		ui.pushButton_start->setToolTip(tr("停止监控"));
		ui.pushButton_setting->setDisabled(true);
		ui.checkBox_cam0->setDisabled(true);
		ui.checkBox_cam1->setDisabled(true);
	}else
	{
		for(int i = 0; i < num; i++)
		{
			if(rtThread[i])
				rtThread[i]->endMonitor();
		}
		isStartMonitor = false;
		QIcon icon;
        icon.addFile(QString::fromUtf8(":/MainWindow/Resources/startanalyze.png"), QSize(), QIcon::Normal, QIcon::Off);
        ui.pushButton_start->setIcon(icon);
		ui.pushButton_start->setToolTip(tr("开始监控"));
		ui.pushButton_setting->setDisabled(false);
		getCameraNum();
	}
}

void MainWindow::on_pushButton_setting_clicked()
{
	realTimeSetting->exec();
}

void MainWindow::on_checkBox_cam0_clicked()
{
	if(ui.checkBox_cam0->isChecked())
	{
		startCamThread(0);
	}else
	{
		endCamThread(0);
	}
}

void MainWindow::on_checkBox_cam1_clicked()
{
	if(ui.checkBox_cam1->isChecked())
	{
		startCamThread(1);
	}else
	{
		endCamThread(1);
	}
}

void MainWindow::on_checkBox_cam2_clicked()
{
	/*
	if(ui.checkBox_cam2->isChecked())
	{
		startCamThread(2);
	}else
	{
		endCamThread(2);
	}
	*/
}

void MainWindow::on_checkBox_cam3_clicked()
{
	/*
	if(ui.checkBox_cam3->isChecked())
	{
		startCamThread(3);
	}else
	{
		endCamThread(3);
	}

	*/
}


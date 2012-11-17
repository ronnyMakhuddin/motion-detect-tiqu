#include "realtimesetting.h"

QString RealTimeSetting::getData()
{
	QString data;
	data += QString::number(scale) + tr("|");
	data += QString::number(min_area) + tr("|");
	data += QString::number(max_area) + tr("|");
	data += QString::number(jiange) + tr("|");
	data += QString::number(fps) + tr("|");
	data += QString::number(max_event) + tr("|");
	data += filePath + tr("/");
	return data;
}

RealTimeSetting::RealTimeSetting(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setFixedSize(506, 372);
	ui.lineEdit_width->setDisabled(true);
	ui.lineEdit_height->setDisabled(true);
	ui.lineEdit_path->setDisabled(true);
	
	scale = 100;
	width = 640;
	height = 480;
	min_area = 1000;
	max_area = 100000;
	jiange = 2;
	fps = 25;
	max_event = 100;
	filePath = tr("D:/realTimeVideo");
	setToUi();
}

RealTimeSetting::~RealTimeSetting()
{

}

//��������ʾ��ui��
void RealTimeSetting::setToUi()
{
	if(scale == 100)
		ui.radioButton100->setChecked(true);
	else if(scale == 75)
		ui.radioButton75->setChecked(true);
	else
		ui.radioButton50->setChecked(true);

	ui.lineEdit_width->setText(QString::number(width));
	ui.lineEdit_height->setText(QString::number(height));
	ui.lineEdit_min_area->setText(QString::number(min_area));
	ui.lineEdit_max_area->setText(QString::number(max_area));
	ui.lineEdit_jiange->setText(QString::number(jiange));

	ui.lineEdit_fps->setText(QString::number(fps));
	ui.lineEdit_max_event_num->setText(QString::number(max_event));
	ui.lineEdit_path->setText(filePath);
}

//��ui������
void RealTimeSetting::readFromUi() 
{
	if(ui.radioButton100->isChecked())
		scale = 100;
	else if(ui.radioButton75->isChecked())
		scale = 75;
	else
		scale = 50;

	width = ui.lineEdit_width->text().toInt();
	height = ui.lineEdit_height->text().toInt();
	min_area = ui.lineEdit_min_area->text().toInt();
	max_area = ui.lineEdit_max_area->text().toInt();
	jiange = ui.lineEdit_jiange->text().toInt();

	fps = ui.lineEdit_fps->text().toInt();
	max_event = ui.lineEdit_max_event_num->text().toInt();
	filePath = ui.lineEdit_path->text();
}

//����ΪĬ������
void RealTimeSetting::setDefault()
{
	/*
	scale = 100;
	width = 640;
	height = 480;
	min_area = 1000;
	max_area = 100000;
	jiange = 2;
	fps = 25;
	max_event = 100;
	filePath = tr("D:\\realTimeVideo\\");
	*/
	ui.radioButton100->setChecked(true);

	ui.lineEdit_width->setText(tr("640"));
	ui.lineEdit_height->setText(tr("480"));
	ui.lineEdit_min_area->setText(tr("1000"));
	ui.lineEdit_max_area->setText(tr("1000000"));
	ui.lineEdit_jiange->setText(tr("2"));

	ui.lineEdit_fps->setText(tr("25"));
	ui.lineEdit_max_event_num->setText(tr("100"));
	ui.lineEdit_path->setText(tr("D:/realTimeVideo"));
}

//ȡ����ť�¼�
void RealTimeSetting::on_pushButton_cancel_clicked()
{
	hide();
}

//ȷ����ť�¼�
void RealTimeSetting::on_pushButton_ok_clicked()
{
	readFromUi();
	//�����ݷ��͸�������

	hide();
}

//Ĭ�����ð�ť�¼�
void RealTimeSetting::on_pushButton_default_clicked()
{
	setDefault();
	//setToUi();
}

//ѡ��·����ť�¼�
void RealTimeSetting::on_pushButton_path_clicked()
{
	QFileDialog::Options options;
	//options != QFileDialog::DontUseNativeDialog;
	QString tempPath = QFileDialog::getExistingDirectory(this, tr("ѡ�񱣴�·��"), filePath, options);
	ui.lineEdit_path->setText(tempPath);
}
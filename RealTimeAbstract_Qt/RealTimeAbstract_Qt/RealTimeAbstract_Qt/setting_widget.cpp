#include "setting_widget.h"

setting_widget::setting_widget(QDialog *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setFixedSize(this->width(), this->height());

	setDefault();

	data.zoom = 100;
	data.pix_width = 0;
	data.pix_height = 0;
	data.min_area = 1000;
	data.max_area = 1000000;
	data.jiange = 1;
	data.fps = 0;
	data.max_event_num = 100;
}

setting_widget::~setting_widget()
{

}

void setting_widget::on_default_pushbutton_clicked()
{
	setDefault();
}

//确定按钮
void setting_widget::on_ok_pushbutton_clicked()
{
	writeData();
	emit send_data(data.zoom, data.pix_width, data.pix_height, data.min_area, data.max_area, data.jiange, data.fps, data.max_event_num);
	this->hide();
	/*
	FILE*fs = fopen("setting.ini", "w+");
	//视频尺寸百分比
	int percent = 100;
	if(ui.radiobutton50->isChecked())
		percent = 50;
	else if(ui.radiobutton75->isChecked())
		percent = 75;
	fprintf(fs, "scope:%d%\n", percent);
	//视频分辨率
	fprintf(fs, "pixels:%s,%s\n", ui.width_lineedit->text().toLocal8Bit().data(), ui.height_lineedit->text().toLocal8Bit().data());
	//目标最小宽、高
	fprintf(fs, "minWidthHeight:%s,%s\n", ui.min_area_lineedit->text().toLocal8Bit().data(), ui.min_area_lineedit->text().toLocal8Bit().data());
	//分析跳帧
	fprintf(fs, "jiange:%s\n", ui.jiange_lineedit->text().toLocal8Bit().data());

	//视频帧率
	fprintf(fs, "fps:%s\n", ui.fps_lineedit->text().toLocal8Bit().data());
	//同一时间最大摘要数
	fprintf(fs, "maxEventNum:%s\n", ui.max_event_num_lineedit->text().toLocal8Bit().data());
	fclose(fs);
	*/
}

void setting_widget::on_cancel_pushbutton_clicked()
{
	hide();
}

void setting_widget::setDefault()
{
	ui.radiobutton100->setChecked(true);
	ui.width_lineedit->setText(tr("0"));
	ui.height_lineedit->setText(tr("0"));
	ui.min_area_lineedit->setText(tr("1000"));
	ui.max_area_lineedit->setText(tr("100000"));
	ui.jiange_lineedit->setText(tr("2"));
	ui.fps_lineedit->setText(tr("15"));
	ui.max_event_num_lineedit->setText(tr("100"));
}

void setting_widget::readData()
{
	if(data.zoom==100)
		ui.radiobutton100->setChecked(true);
	else if(data.zoom == 75)
		ui.radiobutton75->setChecked(true);
	else if(data.zoom == 50)
		ui.radiobutton50->setChecked(true);

	ui.width_lineedit->setText(QString::number(data.pix_width));
	ui.height_lineedit->setText(QString::number(data.pix_height));
	ui.min_area_lineedit->setText(QString::number(data.min_area));
	ui.max_area_lineedit->setText(QString::number(data.max_area));
	ui.jiange_lineedit->setText(QString::number(data.jiange));
	ui.fps_lineedit->setText(QString::number(data.fps));
	ui.max_event_num_lineedit->setText(QString::number(data.max_event_num));
}

void setting_widget::writeData()
{
	if(ui.radiobutton100->isChecked())
		data.zoom = 100;
	else if(ui.radiobutton75->isChecked())
		data.zoom = 75;
	else if(ui.radiobutton50->isChecked())
		data.zoom = 50;

	data.pix_width = ui.width_lineedit->text().toInt();
	data.pix_height = ui.height_lineedit->text().toInt();
	data.min_area = ui.min_area_lineedit->text().toInt();
	data.max_area = ui.max_area_lineedit->text().toInt();
	data.jiange = ui.jiange_lineedit->text().toInt();
	data.max_event_num = ui.max_event_num_lineedit->text().toInt();
}

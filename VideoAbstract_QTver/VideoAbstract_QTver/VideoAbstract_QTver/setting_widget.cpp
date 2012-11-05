#include "setting_widget.h"

setting_widget::setting_widget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setDefault();
}

setting_widget::~setting_widget()
{

}

void setting_widget::on_enter_checkbox_clicked()
{
	if(ui.enter_checkbox->isChecked())
	{//如果被选中
		emit send_enter_checkbox_state(true);  //发送信息给主界面
	}else
	{//如果被取消选中
		ui.enter_checkbox->isChecked();
	}
}

void setting_widget::on_default_pushbutton_clicked()
{
	setDefault();
}

//确定按钮
void setting_widget::on_ok_pushbutton_clicked()
{
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
	fprintf(fs, "minWidthHeight:%s,%s\n", ui.min_width_lineedit->text().toLocal8Bit().data(), ui.min_height_lineedit->text().toLocal8Bit().data());
	//分析跳帧
	fprintf(fs, "jiange:%s\n", ui.jiange_lineedit->text().toLocal8Bit().data());

	//视频帧率
	fprintf(fs, "fps:%s\n", ui.fps_lineedit->text().toLocal8Bit().data());
	//同一时间最大摘要数
	fprintf(fs, "maxEventNum:%s\n", ui.max_event_num_lineedit->text().toLocal8Bit().data());
	fclose(fs);
}

void setting_widget::setDefault()
{
	ui.radiobutton100->setChecked(true);
	ui.width_lineedit->setText(tr("0"));
	ui.height_lineedit->setText(tr("0"));
	ui.min_width_lineedit->setText(tr("50"));
	ui.min_height_lineedit->setText(tr("50"));
	ui.jiange_lineedit->setText(tr("2"));
	ui.fps_lineedit->setText(tr("15"));
	ui.max_event_num_lineedit->setText(tr("100"));

	
}


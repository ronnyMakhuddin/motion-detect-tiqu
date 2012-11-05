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
	{//�����ѡ��
		emit send_enter_checkbox_state(true);  //������Ϣ��������
	}else
	{//�����ȡ��ѡ��
		ui.enter_checkbox->isChecked();
	}
}

void setting_widget::on_default_pushbutton_clicked()
{
	setDefault();
}

//ȷ����ť
void setting_widget::on_ok_pushbutton_clicked()
{
	FILE*fs = fopen("setting.ini", "w+");
	//��Ƶ�ߴ�ٷֱ�
	int percent = 100;
	if(ui.radiobutton50->isChecked())
		percent = 50;
	else if(ui.radiobutton75->isChecked())
		percent = 75;
	fprintf(fs, "scope:%d%\n", percent);
	//��Ƶ�ֱ���
	fprintf(fs, "pixels:%s,%s\n", ui.width_lineedit->text().toLocal8Bit().data(), ui.height_lineedit->text().toLocal8Bit().data());
	//Ŀ����С����
	fprintf(fs, "minWidthHeight:%s,%s\n", ui.min_width_lineedit->text().toLocal8Bit().data(), ui.min_height_lineedit->text().toLocal8Bit().data());
	//������֡
	fprintf(fs, "jiange:%s\n", ui.jiange_lineedit->text().toLocal8Bit().data());

	//��Ƶ֡��
	fprintf(fs, "fps:%s\n", ui.fps_lineedit->text().toLocal8Bit().data());
	//ͬһʱ�����ժҪ��
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


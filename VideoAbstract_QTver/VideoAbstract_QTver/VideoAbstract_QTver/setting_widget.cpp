#include "setting_widget.h"

setting_widget::setting_widget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
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
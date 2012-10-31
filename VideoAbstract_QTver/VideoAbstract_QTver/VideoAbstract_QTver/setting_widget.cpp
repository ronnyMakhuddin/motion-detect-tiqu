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
	{//如果被选中
		emit send_enter_checkbox_state(true);  //发送信息给主界面
	}else
	{//如果被取消选中
		ui.enter_checkbox->isChecked();
	}
}
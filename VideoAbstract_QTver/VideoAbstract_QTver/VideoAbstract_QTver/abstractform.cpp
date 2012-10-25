#include "abstractform.h"

AbstractForm::AbstractForm(int index = -1)
	: QDialog()
{
	this->index = index;
	ui.setupUi(this);
	this->setMinimumSize(QSize(300,255));
}

AbstractForm::~AbstractForm()
{

}

void AbstractForm::destroyMySelf()
{
	ui.image_label->deleteLater();
	ui.end_time_label->deleteLater();
	ui.start_time_label->deleteLater();
	ui.play_button->deleteLater();
}

void AbstractForm::on_play_button_clicked()
{
	
}
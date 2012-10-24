#include "abstractform.h"

AbstractForm::AbstractForm()
	: QDialog()
{
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
	ui.pushButton->deleteLater();
}
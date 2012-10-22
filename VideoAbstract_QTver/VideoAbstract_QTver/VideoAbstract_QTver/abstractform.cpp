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

#include "abstractform.h"

AbstractForm::AbstractForm(int index = -1)
	: QDialog()
{
	this->index = index;
	ui.setupUi(this);
	this->setMinimumSize(QSize(300,255));
	ui.label_abstract_index->setText(tr("ժҪ") + QString::number(index+1));
	

	int ran = cv::getTickCount()%1500;
	if(ran < 500)
		ran += 500;
	QString tempNum;
	ui.label_time->setText(tempNum.setNum(ran)+tr("ms"));
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
	ui.label_time->deleteLater();
}

void AbstractForm::on_play_button_clicked()
{
	sendAbstractPlay(index);
}
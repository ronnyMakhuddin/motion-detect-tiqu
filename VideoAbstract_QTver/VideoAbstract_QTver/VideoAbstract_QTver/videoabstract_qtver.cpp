#include "videoabstract_qtver.h"

VideoAbstract_QTver::VideoAbstract_QTver(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	ui.setupUi(this);
	//this->setWindowFlags( Qt::FramelessWindowHint);
}

VideoAbstract_QTver::~VideoAbstract_QTver()
{

}

void VideoAbstract_QTver::resizeEvent(QResizeEvent*event)
{
	QSize size = this->size();
	ui.widget->move(0, size.height()/2);
	ui.widget->resize(0.4*size.width(), 0.5*size.height());
	QSize subSize = ui.widget->size();
	ui.pushButton->move(0.2*subSize.width(), 0.25*subSize.height());
	ui.pushButton_2->move(0.6*subSize.width(), 0.25*subSize.height());
	ui.pushButton_3->move(0.4*subSize.width(), 0.6*subSize.height());
}
#include "singleabstractlayout.h"

SingleAbstractLayout::SingleAbstractLayout()
	: QVBoxLayout()
{
	pictureLabel = new QLabel();
	textLabel1 = new QLabel();
	textLabel2 = new QLabel();
	QImage image("aa.jpg");
	pictureLabel->setPixmap(QPixmap::fromImage(image));
	textLabel1->setText("line1");
	textLabel2->setText("line2");
	this->addWidget(pictureLabel);
	this->addWidget(textLabel1);
	this->addWidget(textLabel2);
}

void SingleAbstractLayout::destroyMySelf()
{
	pictureLabel->deleteLater();
	textLabel1->deleteLater();
	textLabel2->deleteLater();
}

SingleAbstractLayout::~SingleAbstractLayout()
{

}

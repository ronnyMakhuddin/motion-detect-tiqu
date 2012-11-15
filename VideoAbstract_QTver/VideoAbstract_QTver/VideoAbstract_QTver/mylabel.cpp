#include "mylabel.h"

MyLabel::MyLabel(QWidget *parent)
	: QLabel(parent)
{
	this->setText("mylabeltest@@@@!!!!!");
	//this->setPixmap
}

MyLabel::~MyLabel()
{

}

void MyLabel::mousePressEvent ( QMouseEvent * ev )
{
	this->setText("press Event");
}
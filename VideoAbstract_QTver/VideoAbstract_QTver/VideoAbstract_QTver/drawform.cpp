#include "drawform.h"

drawform::drawform(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui.label->setText("aaa");
	connect(ui.label, SIGNAL(mousePressEvent(QMouseEvent*)), this, SLOT(label_mouse_press(QMouseEvent*)));
}

drawform::~drawform()
{

}

void drawform::label_mouse_press(QMouseEvent * ev)
{
	int i = 0; 
	i = 10-i;
	i++;
}

void drawform::mousePressEvent(QMouseEvent* ev)
{
	//ev->globalX();
	int x = ev->x();
	int y = ev->y();
	y += x;
	x += x;
}
#include "drawform.h"

int DrawForm::FREE = 0;
int DrawForm::PRESSED = 1;
int DrawForm::DRAGED = 2;
int DrawForm::RELEASEED = 3;

DrawForm::DrawForm(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	state = 0;
	ui.label->setText("aaa");
	connect(ui.label, SIGNAL(mousePressEvent(QMouseEvent*)), this, SLOT(label_mouse_press(QMouseEvent*)));
}

DrawForm::~DrawForm()
{

}

void DrawForm::mousePressEvent(QMouseEvent* ev)
{
	//判断是不是在label范围内
	int x = ev->x();
	int y = ev->y();
	if(x >= ui.label->x() && x <= ui.label->x()+ui.label->width() &&
		y >= ui.label->y() && y <= ui.label->y()+ui.label->height())
	{
		state = PRESSED;
		startP.setX(x);
		startP.setY(y);
	}
}

void DrawForm::mouseMoveEvent(QMouseEvent*ev)
{
	if(state == DRAGED || state == PRESSED)
	{
		state = DRAGED;
		int x = ev->x();
		int y = ev->y();
		endP.setX(x);
		endP.setY(y);
		//img.load(tr("windowIcon.png"));
		//ui.label->setPixmap(QPixmap::fromImage(img));
		QString text = tr("startP:") + QString::number(startP.x()) + tr(",") + QString::number(startP.y());
		text = text + tr("   ") +tr("endP:") + QString::number(endP.x()) + tr(",") + QString::number(endP.y());
		ui.label->setText(text);
	}
}

void DrawForm::mouseReleaseEvent(QMouseEvent*ev)
{
	state = FREE;
}

void DrawForm::paintEvent(QPaintEvent*)
{
}
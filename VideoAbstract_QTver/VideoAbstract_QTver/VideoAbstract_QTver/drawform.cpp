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
	baseFrame = 0;
	showFrame = 0;
	qImg = 0;
	ui.image_label->setText("aaa");
	connect(ui.image_label, SIGNAL(mousePressEvent(QMouseEvent*)), this, SLOT(label_mouse_press(QMouseEvent*)));
}

DrawForm::~DrawForm()
{

}

void DrawForm::resizeEvent(QResizeEvent*ev)
{

}

void DrawForm::mousePressEvent(QMouseEvent* ev)
{
	//判断是不是在label范围内
	int x = ev->x();
	int y = ev->y();
	if(x >= ui.image_label->x() && x <= ui.image_label->x()+ui.image_label->width() &&
		y >= ui.image_label->y() && y <= ui.image_label->y()+ui.image_label->height())
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
		//ui.label->setText(text);
		cvCopy(baseFrame, showFrame);
		cvLine(showFrame, cvPoint(startP.x(),startP.y()), cvPoint(endP.x(),endP.y()), cvScalar(255,0,0));
		ui.image_label->setPixmap(QPixmap::fromImage(*qImg));
	}
}

void DrawForm::mouseReleaseEvent(QMouseEvent*ev)
{
	state = FREE;
}

void DrawForm::paintEvent(QPaintEvent*)
{
}

void DrawForm::setBaseFrame(IplImage* frame)
{
	
	if(baseFrame)
	{
		cvReleaseImage(&baseFrame);
		baseFrame = 0;
	}
	if(showFrame)
	{
		cvReleaseImage(&showFrame);
		showFrame = 0;
	}
	baseFrame = cvCreateImage(cvSize(frame->width, frame->height), frame->depth, frame->nChannels);
	ui.image_label->resize(frame->width, frame->height);
	if (frame->origin == IPL_ORIGIN_TL)  
	{  
		cvCopy(frame,baseFrame,0);  
	}  
	else  
	{  
		cvFlip(frame,baseFrame,0);  
	}  
	cvCvtColor(baseFrame,baseFrame,CV_BGR2RGB);
	qImg = new QImage(QSize(frame->width,frame->height), QImage::Format_RGB888);
	showFrame = cvCreateImageHeader(cvSize(frame->width, frame->height),  8, 3);
	showFrame->imageData = (char*)qImg->bits();
	cvCopy(baseFrame, showFrame);
	ui.image_label->setPixmap(QPixmap::fromImage(*qImg));

	//重置控件大小
	this->resize(baseFrame->width, baseFrame->height+100);
	int jianju = (this->width()-3*ui.ok_button->width())/4;
	int y = ui.image_label->height()+12;
	if(jianju <= ui.ok_button->width())
	{
		ui.ok_button->move(jianju, y);
		ui.clear_button->move(ui.ok_button->width()+2*jianju, y);
		ui.groupBox->move(ui.ok_button->width()*2+3*jianju, y);
	}else
	{
		jianju = ui.ok_button->width();
		int bianyuan = (this->width()-3*ui.ok_button->width()-2*jianju)/2;
		ui.ok_button->move(bianyuan, y);
		ui.clear_button->move(bianyuan+ui.ok_button->width()+jianju, y);
		ui.groupBox->move(bianyuan+ui.ok_button->width()*2+jianju*2, y);
	}
}
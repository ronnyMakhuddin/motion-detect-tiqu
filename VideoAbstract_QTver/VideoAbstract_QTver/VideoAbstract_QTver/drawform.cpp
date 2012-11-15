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
	currentLineP1.x = -1;
	currentLineP1.y = -1;
	currentLineP2.x = -1;
	currentLineP2.y = -1;
	currentRectP1.x = -1;
	currentRectP1.y = -1;
	currentRectP2.x = -1;
	currentRectP2.y = -1;

	ui.line_radiobutton->setChecked(true);
	ui.clear_button->setToolTip(tr("aaaaaaa"));
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
		startP.x = x;
		startP.y = y;
	}
}

void DrawForm::mouseMoveEvent(QMouseEvent*ev)
{
	if(state == DRAGED || state == PRESSED)
	{
		state = DRAGED;
		int x = ev->x();
		int y = ev->y();
		//边界判断
		if(x >= ui.image_label->width())
		{
			x = ui.image_label->width()-1;
		}else if(x < 0)
		{
			x = 0;
		}

		if(y >= ui.image_label->height())
		{
			y = ui.image_label->height()-1;
		}else if(y < 0)
		{
			y = 0;
		}
		endP.x = x;
		endP.y = y;
		
		if(ui.line_radiobutton->isChecked())
		{//直线
			//QString text = tr("startP:") + QString::number(startP.x()) + tr(",") + QString::number(startP.y());
			//text = text + tr("   ") +tr("endP:") + QString::number(endP.x()) + tr(",") + QString::number(endP.y());
			//ui.label->setText(text);
			currentLineP1.x = startP.x;
			currentLineP1.y = startP.y;
			currentLineP2.x = endP.x;
			currentLineP2.y = endP.y;
		}else
		{//矩形
			currentRectP1.x = startP.x;
			currentRectP1.y = startP.y;
			currentRectP2.x = endP.x;
			currentRectP2.y = endP.y;
		}

		//画图代码
		reDrawFunction();
	}
}

void DrawForm::mouseReleaseEvent(QMouseEvent*ev)
{
	state = FREE;
}

void DrawForm::on_clear_button_clicked()
{
	currentLineP1.x = -1;
	currentLineP1.y = -1;
	currentLineP2.x = -1;
	currentLineP2.y = -1;
	currentRectP1.x = -1;
	currentRectP1.y = -1;
	currentRectP2.x = -1;
	currentRectP2.y = -1;
	reDrawFunction();
}

void DrawForm::on_ok_button_clicked()
{
	//发送信息给分析类进行事件筛选
	emit sendLineAndRect(currentLineP1, currentLineP2, currentRectP1, currentRectP2);
	this->setShown(false);
}

void DrawForm::reDrawFunction()
{
	cvCopy(baseFrame, showFrame);
	if(currentLineP1.x!=-1)
	{
		//cvLine(showFrame, cvPoint(currentLineP1.x(),currentLineP1.y()), cvPoint(currentLineP2.x(),currentLineP2.y()), cvScalar(255,0,0));
		drawArrow(showFrame,currentLineP1, currentLineP2, 17, 15, Scalar(255,0,0), 1, 4);
	}

	if(currentRectP1.x!=-1)
	{
		cvRectangle(showFrame, currentRectP1, currentRectP2, cvScalar(0,0,255));
	}
	ui.image_label->setPixmap(QPixmap::fromImage(*qImg));
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

void DrawForm::drawArrow(IplImage*& img, Point pStart, Point pEnd, int len, int alpha,             
	Scalar& color, int thickness, int lineType)
{    
	const double PI = 3.1415926;    
	Point arrow;    
	//计算 θ 角
	double angle = atan2((double)(pStart.y - pEnd.y), (double)(pStart.x - pEnd.x));  
	cvLine(img, pStart, pEnd, color, thickness, lineType);   
	//计算箭角边的另一端的端点位置（上面的还是下面的要看箭头的指向，也就是pStart和pEnd的位置） 
	arrow.x = pEnd.x + len * cos(angle + PI * alpha / 180);     
	arrow.y = pEnd.y + len * sin(angle + PI * alpha / 180);  
	cvLine(img, pEnd, arrow, color, thickness, lineType);   
	arrow.x = pEnd.x + len * cos(angle - PI * alpha / 180);     
	arrow.y = pEnd.y + len * sin(angle - PI * alpha / 180);    
	cvLine(img, pEnd, arrow, color, thickness, lineType);
}
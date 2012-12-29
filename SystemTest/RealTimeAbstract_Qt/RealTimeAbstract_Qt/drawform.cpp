#include "drawform.h"

int DrawForm::FREE = 0;
int DrawForm::PRESSED = 1;
int DrawForm::DRAGED = 2;
int DrawForm::RELEASEED = 3;

DrawForm::DrawForm(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	//setFixedSize(width(), height());
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
	connect(ui.image_label, SIGNAL(mousePressEvent(QMouseEvent*)), this, SLOT(label_mouse_press(QMouseEvent*)));

	ui.radioButton_nolimit->setChecked(true);
	ui.radioButton_all->setChecked(true);
	initComboBox_color();
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
	int color = ui.comboBox_color->currentIndex();
	color = color - 1;
	QString minW = ui.lineEdit_minW->text();
	QString maxW = ui.lineEdit_maxW->text();
	QString minH = ui.lineEdit_minH->text();
	QString maxH = ui.lineEdit_maxH->text();
	QString jihe = minW+"_"+maxW+"_"+minH+"_"+maxH;

	int waiguan = -1;
	if(ui.radioButton_wth->isChecked())
	{
		waiguan = 0;
	}else if(ui.radioButton_htw->isChecked())
	{
		waiguan = 1;
	}else if(ui.radioButton_hew->isChecked())
	{
		waiguan = 2;
	}else
	{
		waiguan = -1;
	}

	int leixing = -1;
	if(ui.radioButton_human->isChecked())
	{
		leixing = 0;
	}else if(ui.radioButton_car->isChecked())
	{
		leixing = 1;
	}else
	{
		leixing = -1;
	}


	//发送信息给分析类进行事件筛选
	emit sendLineAndRect(currentLineP1, currentLineP2, currentRectP1, currentRectP2, color, jihe, waiguan, leixing);
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
	ui.frame_1->setFixedSize(baseFrame->width, baseFrame->height+100);
	ui.frame->move(0, baseFrame->height);
	
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

void DrawForm::initComboBox_color()
{
	QStringList texts;
	texts.append(tr("不限"));
	texts.append(tr("红"));
	texts.append(tr("橙"));
	texts.append(tr("黄"));
	texts.append(tr("黄绿"));
	texts.append(tr("绿"));
	texts.append(tr("青绿"));
	texts.append(tr("青"));
	texts.append(tr("蓝绿"));
	texts.append(tr("蓝"));
	texts.append(tr("紫"));
	texts.append(tr("品红"));
	texts.append(tr("紫红"));
	texts.append(tr("黑"));
	ui.comboBox_color->insertItems(0,texts);
}
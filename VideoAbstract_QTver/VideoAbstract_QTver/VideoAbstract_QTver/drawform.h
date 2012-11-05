#ifndef DRAWFORM_H
#define DRAWFORM_H

#include <QDialog>
#include <QMouseEvent>
#include <QPainter>
#include <cv.h>
#include <highgui.h>
#include "ui_drawform.h"
using namespace cv;

class DrawForm : public QDialog
{
	Q_OBJECT

public:
	void setBaseFrame(IplImage*);
	DrawForm(QWidget *parent = 0);
	~DrawForm();

private:
	Ui::drawform ui;
	int state;
private slots:
	void mousePressEvent(QMouseEvent* ev);
	void mouseMoveEvent(QMouseEvent*ev);
	void mouseReleaseEvent(QMouseEvent*ev);
	void paintEvent(QPaintEvent*);
	void on_clear_button_clicked();
	void on_ok_button_clicked();
private:
	void resizeEvent(QResizeEvent*);
	void reDrawFunction();  //ÖØÐÂ»­Í¼
	void drawArrow(IplImage*& img, Point pStart, Point pEnd, int len, int alpha, Scalar& color, int thickness, int lineType);  //»­¼ýÍ·
public:

	QPoint startP;
	QPoint endP;

	QPoint currentLineP1;
	QPoint currentLineP2;
	QPoint currentRectP1;
	QPoint currentRectP2;

	QImage img;

	static int FREE;
	static int PRESSED;
	static int DRAGED;
	static int RELEASEED;

	IplImage*baseFrame;
	IplImage*showFrame;
	QImage *qImg;
};

#endif // DRAWFORM_H

#ifndef DRAWFORM_H
#define DRAWFORM_H

#include <QDialog>
#include <QMouseEvent>
#include <QPainter>
#include "ui_drawform.h"

class DrawForm : public QDialog
{
	Q_OBJECT

public:
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
public:

	QPoint startP;
	QPoint endP;
	QImage img;

	static int FREE;
	static int PRESSED;
	static int DRAGED;
	static int RELEASEED;
};

#endif // DRAWFORM_H

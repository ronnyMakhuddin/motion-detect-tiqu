#ifndef DRAWFORM_H
#define DRAWFORM_H

#include <QDialog>
#include <QMouseEvent>
#include "ui_drawform.h"

class drawform : public QDialog
{
	Q_OBJECT

public:
	drawform(QWidget *parent = 0);
	~drawform();

private:
	Ui::drawform ui;
private slots:
	void label_mouse_press(QMouseEvent* ev);
	void mousePressEvent(QMouseEvent* ev);
};

#endif // DRAWFORM_H

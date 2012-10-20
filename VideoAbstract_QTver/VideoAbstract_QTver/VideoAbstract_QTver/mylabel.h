#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class MyLabel : public QLabel
{
	Q_OBJECT

public:
	MyLabel(QWidget *parent);
	~MyLabel();

private:
	void mousePressEvent ( QMouseEvent * ev );
};

#endif // MYLABEL_H

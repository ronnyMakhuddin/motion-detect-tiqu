#ifndef TEST_H
#define TEST_H

#include <QWidget>
#include "ui_test.h"
#include "mainwindow.h"

class test : public QWidget
{
	Q_OBJECT

public:
	test(QWidget *parent = 0);
	~test();

private:
	Ui::test ui;
	private slots:
	void on_pushButton_clicked();
};

#endif // TEST_H

#include "test.h"

test::test(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

test::~test()
{

}

void test::on_pushButton_clicked()
{
	MainWindow *m(0);
	m->show();
	this->close();
}
#include "videoabstract_qtver.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	VideoAbstract_QTver w;
	w.show();
	return a.exec();
}

#include "mainwindow.h"
#include "videoabstract_qtver.h"
#include <QtGui/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QTextCodec::setCodecForTr( QTextCodec::codecForName("System") );
	QApplication a(argc, argv);
	VideoAbstract_QTver w; 
	//MainWindow w;
	w.show();
	return a.exec();
}

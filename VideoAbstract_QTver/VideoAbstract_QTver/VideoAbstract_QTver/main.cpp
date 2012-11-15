#include "videoabstract_qtver.h"
#include <QtGui/QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
	QTextCodec::setCodecForTr( QTextCodec::codecForName("GBK") );
	QApplication a(argc, argv);
	VideoAbstract_QTver w;
	w.show();
	return a.exec();
}

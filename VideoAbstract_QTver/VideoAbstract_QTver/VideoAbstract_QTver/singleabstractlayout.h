#ifndef SINGLEABSTRACTLAYOUT_H
#define SINGLEABSTRACTLAYOUT_H

#include <QObject>
#include <QVBoxLayout>
#include <qlabel.h>

class SingleAbstractLayout : public QVBoxLayout
{
	Q_OBJECT

public:
	void destroyMySelf();
	SingleAbstractLayout();
	~SingleAbstractLayout();

public:
	QLabel* pictureLabel;
	QLabel* textLabel1;
	QLabel* textLabel2;
};

#endif // SINGLEABSTRACTLAYOUT_H

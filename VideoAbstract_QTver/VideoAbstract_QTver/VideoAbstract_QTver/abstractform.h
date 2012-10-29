#ifndef ABSTRACTFORM_H
#define ABSTRACTFORM_H

#include <QDialog>
#include "ui_abstract.h"

class AbstractForm : public QDialog
{
	Q_OBJECT

public:
	AbstractForm(int);
	~AbstractForm();
	void destroyMySelf();
public:
	Ui::AbstrctForm ui;
	int index;
private slots:
	void on_play_button_clicked();
signals:
	void sendAbstractPlay(int);
};

#endif // ABSTRACTFORM_H

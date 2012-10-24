#ifndef ABSTRACTFORM_H
#define ABSTRACTFORM_H

#include <QDialog>
#include "ui_abstract.h"

class AbstractForm : public QDialog
{
	Q_OBJECT

public:
	AbstractForm();
	~AbstractForm();
	void destroyMySelf();
public:
	Ui::AbstrctForm ui;
};

#endif // ABSTRACTFORM_H

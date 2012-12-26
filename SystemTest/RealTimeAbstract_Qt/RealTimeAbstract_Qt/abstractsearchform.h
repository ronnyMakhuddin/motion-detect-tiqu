#ifndef ABSTRACTSEARCHFORM_H
#define ABSTRACTSEARCHFORM_H

#include <QDialog>
#include "ui_abstractsearchform.h"

class AbstractSearchForm : public QDialog
{
	Q_OBJECT

public:
	AbstractSearchForm(QWidget *parent = 0);
	~AbstractSearchForm();

private:
	Ui::AbstractSearchForm ui;
};

#endif // ABSTRACTSEARCHFORM_H

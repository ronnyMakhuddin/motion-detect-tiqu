#ifndef SETTING_WIDGET_H
#define SETTING_WIDGET_H

#include <QWidget>
#include <ui_setting.h>
#include "FileOperation.h"
class setting_widget : public QWidget
{
	Q_OBJECT

public:
	void setDefault();
	setting_widget(QWidget *parent);
	~setting_widget();

public:
	Ui::setting_ui ui;
private slots:
    void on_enter_checkbox_clicked();
	void on_ok_pushbutton_clicked();
	void on_default_pushbutton_clicked();
signals:
	void send_enter_checkbox_state(bool);
};

#endif // SETTING_WIDGET_H

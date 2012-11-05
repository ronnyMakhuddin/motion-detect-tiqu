#ifndef SETTING_WIDGET_H
#define SETTING_WIDGET_H

#include <QWidget>
#include <ui_setting.h>
#include "FileOperation.h"

struct Settings
{
	int zoom;
	int pix_width;
	int pix_height;
	int min_area;
	int max_area;
	int jiange;

	int fps;
	int max_event_num;
};

class setting_widget : public QWidget
{
	Q_OBJECT

public:
	void setDefault();
	void readData();
	void writeData();
	setting_widget(QWidget *parent);
	~setting_widget();

public:
	Ui::setting_ui ui;
	Settings data;
private slots:
    void on_enter_checkbox_clicked();
	void on_ok_pushbutton_clicked();
	void on_default_pushbutton_clicked();
signals:
	void send_enter_checkbox_state(bool);
	void send_data(int,int,int,int,int,int,int,int);
};

#endif // SETTING_WIDGET_H

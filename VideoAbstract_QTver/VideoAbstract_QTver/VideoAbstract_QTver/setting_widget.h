#ifndef SETTING_WIDGET_H
#define SETTING_WIDGET_H

#include <QWidget>
#include <ui_setting.h>
class setting_widget : public QWidget
{
	Q_OBJECT

public:
	setting_widget(QWidget *parent);
	~setting_widget();

private:
	Ui::setting_ui ui;
private slots:
    void on_enter_checkbox_clicked();
signals:
	void send_enter_checkbox_state(bool);
};

#endif // SETTING_WIDGET_H

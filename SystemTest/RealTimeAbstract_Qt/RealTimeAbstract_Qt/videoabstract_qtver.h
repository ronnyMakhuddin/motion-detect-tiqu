#ifndef VIDEOABSTRACT_QTVER_H
#define VIDEOABSTRACT_QTVER_H

#include <QtGui/QWidget>
#include <QFileDialog>
#include <qmessagebox.h>
#include <QVBoxLayout>
#include "ui_videoabstract_qtver.h"
#include "Globals.h"
#include "VideoAnalyze.h"
#include "setting_widget.h"
#include "abstractform.h"
#include "abstractplayer.h"


class VideoAbstract_QTver : public QWidget
{
	Q_OBJECT

public:
	void batchAnalysis();   //��������Ƶ
	void startTimeCount();  //��ʼ��ʱ
	VideoAbstract_QTver(QWidget *parent = 0, Qt::WFlags flags = 0);
	~VideoAbstract_QTver();

protected:
	void resizeEvent(QResizeEvent*event);

private slots:
	void on_open_file_button_clicked();
	void on_analysis_button_clicked();
	void on_setting_button_clicked();
	void on_pushButton_switch_clicked();
	void on_search_button_clicked();
	void on_pushButton_playAll_clicked();
	void on_show_video_check_box_clicked();
	void showVideo(QImage, int);
	void openFileFailed();
	void updateProcessBar(int);
	void drawAbstracts(QImage,QString,QString,int);
	void updateProcessInfo(QString);
	void changeAnalyzeButton(QString);  //������ť����
	void changeAnalyzeButtonState(int);       //������ť״̬
	void updateRunTime();        //���·���ʱ��
	void endTimeCount();         //������ʱ
	void updateEventCount(int);     //�����¼�����
	void playAbstract(int);       //����ѡ�е�ժҪ�¼�
	void removeAllAbstracts();    //ɾ������ժҪ�¼�

	void get_enter_checkbox_state(bool);

signals:
	void sendSwitchToRealTimeForm();

public:
	VideoAnalyze *analyzeThread;
private:
	Ui::VideoAbstract_QTverClass ui;
	setting_widget*settingUI;
	QGridLayout*gLayout;
	AbstractPlayer* player;
	DrawForm* drawForm;

	int testInt;

	QTimer*timer;
	int runSeconds;
	//MyLabel* myLabel;
};

#endif // VIDEOABSTRACT_QTVER_H

#include "videoabstract_qtver.h"

//����Ƶ��ť
void VideoAbstract_QTver::on_open_file_button_clicked()
{
	QFileDialog::Options options;
    //if (!native->isChecked())
       // options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    QStringList files = QFileDialog::getOpenFileNames(
		this, tr("QFileDialog::getOpenFileNames()"),
		QDir::currentPath(),
        tr("��Ƶ�ļ� (*.avi)"),
        &selectedFilter,
        options);
    if (files.count()==1) //��һ���ļ�
	{
        //analysis file
	}else if(files.count()>1)  //�򿪶���ļ�
	{
	}else
	{
	}
}

VideoAbstract_QTver::VideoAbstract_QTver(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	ui.setupUi(this);
	//this->setWindowFlags( Qt::FramelessWindowHint);
}

VideoAbstract_QTver::~VideoAbstract_QTver()
{

}

void VideoAbstract_QTver::resizeEvent(QResizeEvent*event)
{
	{
		//3��widget���ڵĲ���
		QSize size = this->size();
		int x = (int)(4.0*size.width()/9);
		int y = (int)(0.5*size.height());
		ui.left_widget_up->move(0, 0);
		ui.left_widget_up->resize(x, y);
		ui.left_widget_down->move(0, y);
		ui.left_widget_down->resize(x, size.height()-y);
		ui.right_widget->move(x, 0);
		ui.right_widget->resize(size.width()-x, size.height());
		ui.line_h->move(0, y);
		ui.line_h->resize(x, 3);
		ui.line_v->move(x, 0);
		ui.line_v->resize(3, size.height());
	}

	{
		//���´����пؼ�����
		QSize size = ui.left_widget_down->size();
		ui.progress_bar->resize(size.width()-ui.progress_bar->x()-5, 30);
		ui.progress_info->resize(size.width()-10, 65);
		int buttonWidth = ui.end_button->width();
		int buttonHeight = ui.end_button->height();
		int y_button = ui.progress_info->y()+ui.progress_info->height();
		int x_jiange = (int)((1.0*size.width()-3*buttonWidth)/4);
		int y_jiange = (int)( ((1.0*size.height()-(ui.progress_info->y()+ui.progress_info->height())) - 2*buttonHeight)/3 );
		ui.open_file_button->move(x_jiange, y_jiange+y_button);
		ui.analysis_button->move(buttonWidth+2*x_jiange, y_jiange+y_button);
		ui.end_button->move(2*buttonWidth+3*x_jiange, y_jiange+y_button);
		ui.show_button->move(x_jiange, buttonHeight+2*y_jiange+y_button);
		ui.search_button->move(buttonWidth+2*x_jiange, buttonHeight+2*y_jiange+y_button);
		ui.setting_button->move(2*buttonWidth+3*x_jiange, buttonHeight+2*y_jiange+y_button);
	}
}
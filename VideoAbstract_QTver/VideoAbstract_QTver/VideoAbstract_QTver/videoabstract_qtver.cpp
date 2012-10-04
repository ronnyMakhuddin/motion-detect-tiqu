#include "videoabstract_qtver.h"

//打开视频按钮
void VideoAbstract_QTver::on_open_file_button_clicked()
{
	QFileDialog::Options options;
    //if (!native->isChecked())
       // options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    Globals::files = QFileDialog::getOpenFileNames(
		this, tr("QFileDialog::getOpenFileNames()"),
		QDir::currentPath(),
        tr("视频文件 (*.avi)"),
        &selectedFilter,
        options);
	
    if(Globals::files.count()==1) //打开一个文件
	{
        QString filePath = tr("文件路径:") + Globals::files[0];
		ui.progress_info->setText(filePath);
	}else if(Globals::files.count()>1)  //打开多个文件
	{
		QString fileDir;
		Globals::getFileDirFromQString(Globals::files[0], fileDir);
		QString text = tr("打开了") + QString::number(Globals::files.count()) + tr("个文件\n") +
			tr("文件目录为:") + fileDir;
		ui.progress_info->setText(text); 
	}else
	{
	}
}

//分析视频按钮
void VideoAbstract_QTver::on_analysis_button_clicked()
{
    if(Globals::files.count()==1) //打开一个文件
	{
        QString filePath = tr("文件路径:") + Globals::files[0];
		ui.progress_info->setText(filePath);
		analyzeThread = new VideoAnalyze(this);
		analyzeThread->filePath = Globals::files[0];
		connect(analyzeThread, SIGNAL(sendQImage(QImage,int)), this, SLOT(showVideo(QImage,int)));
		analyzeThread->start();
	}else if(Globals::files.count()>1)  //打开多个文件
	{
		QString fileDir;
		Globals::getFileDirFromQString(Globals::files[0], fileDir);
		QString text = tr("打开了") + QString::number(Globals::files.count()) + tr("个文件\n") +
			tr("文件目录为:") + fileDir;
		ui.progress_info->setText(text); 
	}else  //弹出对话框，提示请选择文件
	{
		QMessageBox::warning(this, tr("错误"), tr("请先选择视频文件！"));
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
		//3个widget窗口的布局
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

	//左上窗口控件布局
	{
		ui.video_label->resize(ui.left_widget_up->width(), ui.left_widget_up->height());
	}

	{
		//左下窗口中控件布局
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

void VideoAbstract_QTver::showVideo(QImage qImage, int value)
{
	QImage newImg = qImage.scaled(ui.video_label->width(), ui.video_label->height());
	ui.video_label->setPixmap(QPixmap::fromImage(newImg));
	ui.progress_bar->setValue(value);
}
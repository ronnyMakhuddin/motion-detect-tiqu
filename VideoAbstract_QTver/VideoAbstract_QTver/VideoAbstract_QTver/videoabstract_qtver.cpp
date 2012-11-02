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
	startTimeCount();
	if(!analyzeThread->isContinue)
	{
		//清除摘要事件列表控件
		QLayoutItem* child;
		while ((child = gLayout->takeAt(0)) != 0) 
		{
			//((AbstractForm*)child)->destroyMySelf();
			child->widget()->deleteLater();
			delete child;
		}
		gLayout->update();
	
		if(Globals::files.count()==1) //打开一个文件
		{
			QString filePath = tr("文件路径:") + Globals::files[0];
			ui.progress_info->setText(filePath);
			analyzeThread->filePath = Globals::files[0];
			if(ui.show_video_check_box->isChecked())
			{
				analyzeThread->isShowVideo = true;
			}else
			{
				analyzeThread->isShowVideo = false;
			}

			//判断分析文件是否存在
			QString fileDir, fileName;
			Globals::getFileDirFromQString(analyzeThread->filePath, fileDir);
			Globals::getFileNameFromQString(analyzeThread->filePath, fileName);
			fileDir = fileDir + tr("analyze\\");
			QString analyzeFilePath = fileDir + fileName + tr(".txt");
			QFile file(analyzeFilePath);
			if(file.exists() && !QMessageBox::information(this, tr("请选择"), tr("存在分析文件，是否读入？"), tr("读入"), tr("不读入")))
			{//存在就从文件读取摘要信息
				analyzeThread->isReadFromFile = true;
				//FileOperation::readFromFile(analyzeFilePath, analyzeThread->jiange, analyzeThread->fps, analyzeThread->key_jiange, analyzeThread->eventList);
				analyzeThread->start();
			}else
			{
				analyzeThread->isReadFromFile = false;
				analyzeThread->start();
			}
			ui.analysis_button->setText(tr("结束分析"));
		}else if(Globals::files.count()>1)  //打开多个文件
		{
			analyzeThread->filePathList = Globals::files;
			if(ui.show_video_check_box->isChecked())
			{
				analyzeThread->isShowVideo = true;
			}else
			{
				analyzeThread->isShowVideo = false;
			}

			analyzeThread->isBatch = true;
			analyzeThread->isIgnoreExistAnalyze = QMessageBox::information(this, tr("请选择"), tr("已经分析过的视频是否略过？"), tr("不略过"), tr("略过"));

			analyzeThread->start();
			ui.analysis_button->setText(tr("结束分析"));
			//batchAnalysis();
		}else  //弹出对话框，提示请选择文件
		{
			QMessageBox::warning(this, tr("错误"), tr("请先选择视频文件！"));
		}
	}else
	{
		analyzeThread->isContinue = false;
		ui.analysis_button->setText(tr("开始分析"));
	}
}

//是否显示视频复选框
void VideoAbstract_QTver::on_show_video_check_box_clicked()
{
	if(analyzeThread)
	{
		if(ui.show_video_check_box->isChecked())
		{
			analyzeThread->isShowVideo = true;
		}else
		{
			analyzeThread->isShowVideo = false;
		}
	}
}

//摘要检索按钮
void VideoAbstract_QTver::on_search_button_clicked()
{
	/*
	QLayoutItem* child;
	while ((child = gLayout->takeAt(0)) != 0) 
	{
		((SingleAbstractLayout*)child)->destroyMySelf();
		gLayout->removeItem(child);
		
		//child->widget()->deleteLater();
		//delete child;
	}
	gLayout->update();
	*/

	/*
	//时事摘要分析测试代码
	analyzeThread->isRealTime = true;
	if(ui.show_video_check_box->isChecked())
	{
		analyzeThread->isShowVideo = true;
	}else
	{
		analyzeThread->isShowVideo = false;
	}
	analyzeThread->start();
	*/
}

//设置按钮
void VideoAbstract_QTver::on_setting_button_clicked()
{
	settingUI->show();
}

VideoAbstract_QTver::VideoAbstract_QTver(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	ui.setupUi(this);
	analyzeThread = 0;
	gLayout = new QGridLayout();
	gLayout->setSpacing(1);
	gLayout->setMargin(1);
	ui.scrollAreaWidgetContents->setLayout(gLayout);
	timer = new QTimer(0);

	testInt = 0;
	// 测试label点击事件代码
	//myLabel = new MyLabel(this);

	analyzeThread = new VideoAnalyze(this);
	connect(analyzeThread, SIGNAL(sendQImage(QImage,int)), this, SLOT(showVideo(QImage,int)));
	connect(analyzeThread, SIGNAL(sendOpenFileFailed()), this, SLOT(openFileFailed()));
	connect(analyzeThread, SIGNAL(sendProcessBarValue(int)), this, SLOT(updateProcessBar(int)));
	connect(analyzeThread, SIGNAL(sendDrawAbstracts(QImage,QString,QString,int)), this, SLOT(drawAbstracts(QImage,QString,QString,int)));
	connect(analyzeThread, SIGNAL(sendProcessInfo(QString)), this, SLOT(updateProcessInfo(QString)));
	connect(analyzeThread, SIGNAL(sendChangeAnalyzeButtonText(QString)), this, SLOT(changeAnalyzeButton(QString)));
	connect(analyzeThread, SIGNAL(sendRunTime(QString)), this, SLOT(updateRunTime(QString)));
	connect(analyzeThread, SIGNAL(sendEndTimeCount()), this, SLOT(endTimeCount()));
	connect(analyzeThread, SIGNAL(sendEventCount(int)), this, SLOT(updateEventCount(int)));
	
	player = new AbstractPlayer();   //播放器

	drawForm = new DrawForm(this);       //画线界面

	settingUI = new setting_widget(0);   //设置界面
	connect(settingUI, SIGNAL(send_enter_checkbox_state(bool)), this, SLOT(get_enter_checkbox_state(bool)));
}

VideoAbstract_QTver::~VideoAbstract_QTver()
{

}

void VideoAbstract_QTver::resizeEvent(QResizeEvent*event)
{/*
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

	//右边窗口布局
	{
		QSize size = ui.right_widget->size();
		ui.scrollArea->resize(size.width(), size.height()-ui.label_2->height());
	}
*/
}

void VideoAbstract_QTver::showVideo(QImage qImage, int value)
{
	QImage newImg = qImage.scaled(ui.video_label->width(), ui.video_label->height());
	ui.video_label->setPixmap(QPixmap::fromImage(newImg));
	ui.progress_bar->setValue(value);
}

void VideoAbstract_QTver::drawAbstracts(QImage newImage, QString start, QString end, int count)
{
	int row = 0, col = 0;
	if(count % 2 == 1)
		col = 1;
	row = count / 2;
	AbstractForm*form = new AbstractForm(count);
	connect(form, SIGNAL(sendAbstractPlay(int)), this, SLOT(playAbstract(int)));
	form->ui.image_label->setPixmap(QPixmap::fromImage(newImage));
	form->ui.start_time_label->setText(start);
	form->ui.end_time_label->setText(end);
	gLayout->addWidget(form, row, col);
}

void VideoAbstract_QTver::updateProcessInfo(QString info)
{
	ui.progress_info->setText(info);
}

void VideoAbstract_QTver::updateRunTime()
{
	this->runSeconds++;
	int h = runSeconds / 3600;
	int m = (runSeconds % 3600) / 60;
	int s = (runSeconds % 60);
	QString time = tr("耗时:") + QString::number(h, 10) + tr("时") + QString::number(m, 10) + tr("分") + QString::number(s, 10) + tr("秒");
	ui.progress_label->setText(time);
}

void VideoAbstract_QTver::openFileFailed()
{
	QMessageBox::warning(this, tr("错误"), tr("视频文件损坏或格式不正确，无法打开！"));
}

void VideoAbstract_QTver::updateProcessBar(int value)
{
	ui.progress_bar->setValue(value);
}

void VideoAbstract_QTver::changeAnalyzeButton(QString text)
{
	ui.analysis_button->setText(text);
}

void VideoAbstract_QTver::updateEventCount(int num)
{
	QString str = tr("事件数量:") + QString::number(num, 10);
	ui.event_count_label->setText(str);
}

void VideoAbstract_QTver::playAbstract(int index)
{
	player->show();
	player->thread->init(analyzeThread->filePath, analyzeThread->eventList[index], analyzeThread->jiange);
	player->thread->start();
	player->thread->isPlaying = true;
	player->ui.play_button->setText(tr("暂停"));
	//connect(analyzeThread, SIGNAL(sendQImage(QImage,int)), player, SLOT(showImage(QImage)));
	//disconnect(analyzeThread, SIGNAL(sendQImage(QImage,int)), player, SLOT(showImage(QImage)));
}

void VideoAbstract_QTver::get_enter_checkbox_state(bool isCheck)
{
	if(isCheck)
	{
		if(analyzeThread->filePath.size()<=1)
		{
			QMessageBox::warning(this, tr("错误"), tr("请先选择视频文件！"));
			return;
		}
		drawForm->setBaseFrame(analyzeThread->getFrameByNumber(18));
		drawForm->show();
	}else
	{
	}
}

void VideoAbstract_QTver::batchAnalysis()
{
	bool isIgnore = false;
	isIgnore = QMessageBox::information(this, tr("请选择"), tr("已经分析过的视频是否略过？"), tr("不略过"), tr("略过"));
	for(int i = 0; i < Globals::files.size(); i++)
	{
		QString info = tr("正在分析第") + QString(i+1) + tr("个文件，共") + QString(Globals::files.size()) + tr("个文件。");
		ui.progress_info->setText(info);
		if(analyzeThread)
		{
			delete analyzeThread;
			analyzeThread = 0;
		}
		analyzeThread = new VideoAnalyze(this);
		analyzeThread->filePath = Globals::files[i];
		if(ui.show_video_check_box->isChecked())
		{
			analyzeThread->isShowVideo = true;
		}else
		{
			analyzeThread->isShowVideo = false;
		}
		connect(analyzeThread, SIGNAL(sendQImage(QImage,int)), this, SLOT(showVideo(QImage,int)));
		connect(analyzeThread, SIGNAL(sendOpenFileFailed()), this, SLOT(openFileFailed()));
		connect(analyzeThread, SIGNAL(sendProcessBarValue(int)), this, SLOT(updateProcessBar(int)));
		connect(analyzeThread, SIGNAL(sendDrawAbstracts(QImage,QString,QString)), this, SLOT(drawAbstracts(QImage,QString,QString)));

		//判断分析文件是否存在
		QString fileDir, fileName;
		Globals::getFileDirFromQString(analyzeThread->filePath, fileDir);
		Globals::getFileNameFromQString(analyzeThread->filePath, fileName);
		fileDir = fileDir + tr("analyze\\");
		QString analyzeFilePath = fileDir + fileName + tr(".txt");
		QFile file(analyzeFilePath);
		if(file.exists() && isIgnore)
		{//存在就从文件读取摘要信息
			analyzeThread->isReadFromFile = true;
			FileOperation::readFromFile(analyzeFilePath, analyzeThread->jiange, analyzeThread->fps, analyzeThread->key_jiange, analyzeThread->eventList);
			analyzeThread->start();
		}else
		{
			analyzeThread->start();
		}
		while(!analyzeThread->isFinished())
		{
			;
		}
		
	}
}

void VideoAbstract_QTver::startTimeCount()
{
	runSeconds = 0;
	//timer = new QTimer(0);  
	timer->setInterval(1000);  
	connect(timer,SIGNAL(timeout()),this,SLOT(updateRunTime()));  
	timer->start();
}

void VideoAbstract_QTver::endTimeCount()
{
	if(timer)
	{
		timer->stop();
	}
}
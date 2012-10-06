#include "videoabstract_qtver.h"

//����Ƶ��ť
void VideoAbstract_QTver::on_open_file_button_clicked()
{
	QFileDialog::Options options;
    //if (!native->isChecked())
       // options |= QFileDialog::DontUseNativeDialog;
    QString selectedFilter;
    Globals::files = QFileDialog::getOpenFileNames(
		this, tr("QFileDialog::getOpenFileNames()"),
		QDir::currentPath(),
        tr("��Ƶ�ļ� (*.avi)"),
        &selectedFilter,
        options);
	
    if(Globals::files.count()==1) //��һ���ļ�
	{
        QString filePath = tr("�ļ�·��:") + Globals::files[0];
		ui.progress_info->setText(filePath);
	}else if(Globals::files.count()>1)  //�򿪶���ļ�
	{
		QString fileDir;
		Globals::getFileDirFromQString(Globals::files[0], fileDir);
		QString text = tr("����") + QString::number(Globals::files.count()) + tr("���ļ�\n") +
			tr("�ļ�Ŀ¼Ϊ:") + fileDir;
		ui.progress_info->setText(text); 
	}else
	{
	}
}

//������Ƶ��ť
void VideoAbstract_QTver::on_analysis_button_clicked()
{
	//���ժҪ�¼��б�ؼ�
	QLayoutItem* child;
	while ((child = vLayout->takeAt(0)) != 0) 
	{
		child->widget()->deleteLater();
		delete child;
	}
	vLayout->update();

    if(Globals::files.count()==1) //��һ���ļ�
	{
        QString filePath = tr("�ļ�·��:") + Globals::files[0];
		ui.progress_info->setText(filePath);
		analyzeThread = new VideoAnalyze(this);
		analyzeThread->filePath = Globals::files[0];
		if(0==ui.show_button->text().compare(tr("��ʾ��������")))
		{
			analyzeThread->isShowVideo = false;
		}else
		{
			analyzeThread->isShowVideo = true;
		}
		connect(analyzeThread, SIGNAL(sendQImage(QImage,int)), this, SLOT(showVideo(QImage,int)));
		connect(analyzeThread, SIGNAL(sendOpenFileFailed()), this, SLOT(openFileFailed()));
		connect(analyzeThread, SIGNAL(sendProcessBarValue(int)), this, SLOT(updateProcessBar(int)));
		connect(analyzeThread, SIGNAL(sendDrawAbstracts(QImage,QString,QString)), this, SLOT(drawAbstracts(QImage,QString,QString)));

		//�жϷ����ļ��Ƿ����
		QString fileDir, fileName;
		Globals::getFileDirFromQString(analyzeThread->filePath, fileDir);
		Globals::getFileNameFromQString(analyzeThread->filePath, fileName);
		fileDir = fileDir + tr("analyze\\");
		QString analyzeFilePath = fileDir + fileName + tr(".txt");
		QFile file(analyzeFilePath);
		if(file.exists() && !QMessageBox::information(this, tr("��ѡ��"), tr("���ڷ����ļ����Ƿ���룿"), tr("����"), tr("������")))
		{//���ھʹ��ļ���ȡժҪ��Ϣ
			analyzeThread->isReadFromFile = true;
			FileOperation::readFromFile(analyzeFilePath, analyzeThread->jiange, analyzeThread->fps, analyzeThread->key_jiange, analyzeThread->eventList);
			analyzeThread->start();
		}else
		{
			analyzeThread->start();
		}
	}else if(Globals::files.count()>1)  //�򿪶���ļ�
	{
		QString fileDir;
		Globals::getFileDirFromQString(Globals::files[0], fileDir);
		QString text = tr("����") + QString::number(Globals::files.count()) + tr("���ļ�\n") +
			tr("�ļ�Ŀ¼Ϊ:") + fileDir;
		ui.progress_info->setText(text); 
	}else  //�����Ի�����ʾ��ѡ���ļ�
	{
		QMessageBox::warning(this, tr("����"), tr("����ѡ����Ƶ�ļ���"));
	}
}

//�Ƿ���ʾ��Ƶ��ť
void VideoAbstract_QTver::on_show_button_clicked()
{
	if(analyzeThread)
	{
		if(analyzeThread->isShowVideo)
		{
			analyzeThread->isShowVideo = false;
			ui.show_button->setText(tr("��ʾ��������"));
		}else
		{
			analyzeThread->isShowVideo = true;
			ui.show_button->setText(tr("�رշ�������"));
		}
	}
}

VideoAbstract_QTver::VideoAbstract_QTver(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	ui.setupUi(this);
	analyzeThread = 0;
	vLayout = new QVBoxLayout();
	ui.scrollAreaWidgetContents->setLayout(vLayout);
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

	//���ϴ��ڿؼ�����
	{
		ui.video_label->resize(ui.left_widget_up->width(), ui.left_widget_up->height());
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

void VideoAbstract_QTver::showVideo(QImage qImage, int value)
{
	QImage newImg = qImage.scaled(ui.video_label->width(), ui.video_label->height());
	ui.video_label->setPixmap(QPixmap::fromImage(newImg));
	ui.progress_bar->setValue(value);
}

void VideoAbstract_QTver::drawAbstracts(QImage qImg, QString start, QString end)
{
	QImage newImage = qImg.copy();
	SingleAbstractLayout*layout = new SingleAbstractLayout();
	layout->pictureLabel->setPixmap(QPixmap::fromImage(newImage));
	layout->textLabel1->setText(start);
	layout->textLabel2->setText(end);
	vLayout->addLayout(layout);
}

void VideoAbstract_QTver::openFileFailed()
{
	QMessageBox::warning(this, tr("����"), tr("��Ƶ�ļ��𻵻��ʽ����ȷ���޷��򿪣�"));
}

void VideoAbstract_QTver::updateProcessBar(int value)
{
	ui.progress_bar->setValue(value);
}
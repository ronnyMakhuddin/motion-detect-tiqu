#include "VideoAnalyze.h"

void VideoAnalyze::run()
{
	if(isBatch)
	{
		this->batchAnalysis();
	}else
	{
		if(!isReadFromFile)
		{
			isSaveToFile = false;
			this->analyzeVideo();
			this->saveEventToFile();
			this->drawAbstracts();
			this->createAllEventVideo();
		}else
		{
			isSaveToFile = true;
			this->getBaseFrame();
			this->drawAbstracts();
			//this->createAllEventVideo();
		}

		if(capture)
		{
			//getKeyFrameJiange();   opencv2.4.0���Զ���ȷ��λ֡�����ô˺���
			cvReleaseCapture(&capture);
		}
		if(qImg)
			delete qImg;
		if(iplImg)
			cvReleaseImage(&iplImg);
	}


}

void VideoAnalyze::update_mhi(IplImage*&img, IplImage*&dst, int frameNum, IplImage**&buf, int&last, IplImage*&mhi, CvSize size, double&lastTime)
{
	int N = 3;
	double MHI_DURATION = 0.5;//0.5sΪ�˶����ٵ�������ʱ��
	double timestamp = clock()/100.0; // get current time in seconds ʱ���
    int i, idx1, idx2;
    IplImage* silh;
    IplImage* pyr = cvCreateImage( cvSize((size.width & -2)/2, (size.height & -2)/2), 8, 1 );
    CvMemStorage *stor;
    CvSeq *cont;
 
    //�Ƚ������ݵĳ�ʼ��
    if( !mhi || mhi->width != size.width || mhi->height != size.height )
    {
		if( buf == 0 ) //����û�г�ʼ��������ڴ����
        {
            buf = (IplImage**)malloc(N*sizeof(buf[0]));
            memset( buf, 0, N*sizeof(buf[0]));
        }
        
        for( i = 0; i < N; i++ )
        {
            cvReleaseImage( &buf[i] );
            buf[i] = cvCreateImage( size, IPL_DEPTH_8U, 1 );
            cvZero( buf[i] );// clear Buffer Frame at the beginning
        }
        cvReleaseImage( &mhi );
        mhi = cvCreateImage( size, IPL_DEPTH_32F, 1 );
        cvZero( mhi ); // clear MHI at the beginning
    } // end of if(mhi)
    /*����ǰҪ�����֡ת��Ϊ�Ҷȷŵ�buffer�����һ֡��*/
    cvCvtColor( img, buf[last], CV_BGR2GRAY ); // convert frame to grayscale

    idx1 = last;
    idx2 = (last + 1) % N; // index of (last - (N-1))th frame
    last = idx2;
    // ��֡��
    silh = buf[idx2];//��ֵ��ָ��idx2 |idx2-idx1|-->idx2(<-silh)
    cvAbsDiff( buf[idx1], buf[idx2], silh ); // get difference between frames
    
    // �Բ�ͼ������ֵ��
    cvThreshold( silh, silh, 30, 255, CV_THRESH_BINARY ); //threshold it,��ֵ��
    
    cvUpdateMotionHistory( silh, mhi, timestamp, MHI_DURATION ); // update MHI
 
    cvConvert( mhi, dst );//��mhiת��Ϊdst,dst=mhi   
    
    // ��ֵ�˲�������С������
    cvSmooth( dst, dst, CV_MEDIAN, 3, 0, 0, 0 );
    
    
    cvPyrDown( dst, pyr, CV_GAUSSIAN_5x5 );// ���²�����ȥ��������ͼ����ԭͼ����ķ�֮һ
    cvDilate( pyr, pyr, 0, 1 );  // �����Ͳ���������Ŀ��Ĳ������ն�
    cvPyrUp( pyr, dst, CV_GAUSSIAN_5x5 );// ���ϲ������ָ�ͼ��ͼ����ԭͼ����ı�
	
    //
    // ����ĳ���������ҵ�����
    //
    // Create dynamic structure and sequence.
    stor = cvCreateMemStorage(0);
    cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor);
    
    // �ҵ���������
    cvFindContours( dst, stor, &cont, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
	for (; cont; cont = cont->h_next)
	{
		CvRect r = ((CvContour*)cont)->rect;
		if (r.height * r.width > minArea && r.height * r.width < maxArea)
		{
			CvScalar s;

			EventNode node;
			if (eventList.size() == 0)
			{
				node = EventNodeOperation::insertEventNode(eventList, r, frameNum);
			}
			else
			{
				
				if(!EventNodeOperation::searchEventList(eventList, r, node))
					node = EventNodeOperation::insertEventNode(eventList, r, frameNum);
			}
			s = EventNodeOperation::sampleColor[node.startFrame % 5];

			cvRectangle(img, cvPoint(r.x, r.y), cvPoint(r.x + r.width, r.y + r.height), s, 1, CV_AA, 0);
		}
	}

	EventNodeOperation::bianliEventList(eventList, frameNum);

	cvReleaseMemStorage(&stor);
	cvReleaseImage(&pyr);
}

void VideoAnalyze::batchAnalysis()
{
	for(int i = 0; i < filePathList.size(); i++)
	{
		filePath = filePathList[i];
		//�������ڷ�������Ƶ��Ϣ
		QString info = tr("���ڷ�����") + QString::number(i+1, 10) + tr("���ļ�����") + QString::number(Globals::files.size(), 10) + tr("���ļ���");
		emit sendProcessInfo(info);

		//�жϷ����ļ��Ƿ����
		QString fileDir, fileName;
		Globals::getFileDirFromQString(filePath, fileDir);
		Globals::getFileNameFromQString(filePath, fileName);
		fileDir = fileDir + tr("analyze\\");
		QString analyzeFilePath = fileDir + fileName + tr(".txt");
		QFile file(analyzeFilePath);
		if(file.exists() && this->isIgnoreExistAnalyze)
			continue;

		if(!isReadFromFile)
		{
			isSaveToFile = false;
			this->analyzeVideo();
			this->saveEventToFile();
			//this->drawAbstracts();
		}else
		{
			isSaveToFile = true;
			//this->drawAbstracts();
		}

		if(capture)
		{
			cvReleaseCapture(&capture);
		}
		if(qImg)
			delete qImg;
		if(iplImg)
			cvReleaseImage(&iplImg);
	}
}

void VideoAnalyze::analyzeVideo()
{
	int N = 3;
	QByteArray ba = filePath.toLocal8Bit();
	const char *file = ba.data();
	capture = cvCaptureFromAVI(file);
	IplImage*motion = 0;   //�ڴ�δ�ͷţ�
	IplImage**buf = 0;
	IplImage*mhi = 0;
	int last = 0;
	double lastTime = 0;
	if(capture)
	{
		captureSize = cvSize((int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
			(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
		fps = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
		frameCount = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);

		qImg = new QImage(QSize(captureSize.width,captureSize.height), QImage::Format_RGB888);
		iplImg = cvCreateImageHeader(captureSize,  8, 3);
		iplImg->imageData = (char*)qImg->bits();
		baseFrame = cvCreateImage(captureSize,  8, 3);
		
		minArea = 100;
		maxArea = 1000000;

		mhi = cvCreateImage(captureSize, IPL_DEPTH_32F, 1);
		cvZero(mhi);
		buf = new IplImage*[N];
		for (int i = 0; i < N; i++)
		{
			//cvReleaseImage(&buf[i]);
			buf[i] = cvCreateImage(captureSize, IPL_DEPTH_8U, 1);
			cvZero(buf[i]);
		}
		int totalFrames = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
		isSaveToFile = true;
		int frameNum = 0;
		while (true)
		{
			frame = cvQueryFrame(capture);
			if (!frame || !isContinue)
			{
				emit sendProcessBarValue(100);
				msleep(100);//�����������ͣ�Ļ�������Ϊ��ʵͼƬ���ͷŶ������ڴ����
				break;
			}
			//��ȡ����֡ΪbaseFrame
			if(frameNum == 5)
				cvCopy(frame, baseFrame);
			if (frameNum % jiange == 0)
			{
				if (frame)
				{
					if (!motion)
					{
						motion = cvCreateImage(captureSize, IPL_DEPTH_8U, 1);
						cvZero(motion);
					}
				}
				update_mhi(frame, motion, frameNum, buf, last, mhi, captureSize, lastTime);
				if (frame)  
				{  
					if (frame->origin == IPL_ORIGIN_TL)  
					{  
						cvCopy(frame,iplImg,0);  
					}  
					else  
					{  
						cvFlip(frame,iplImg,0);  
					}  
					cvCvtColor(iplImg,iplImg,CV_BGR2RGB);
				}  
				if(isContinue)
				{
					int value = (int)(100.0*(1+frameNum)/frameCount);
					if(isShowVideo)
					{
						emit sendQImage(*qImg, value);
						msleep(10);
					}else
					{
						emit sendProcessBarValue(value);
					}
				}
			}
			frameNum++;
		}
	}
	else
	{
		emit sendOpenFileFailed();
	}

}

void VideoAnalyze::createAllEventVideo()
{
	//if(!capture || !isSaveToFile)
		//return;

	QString videoPath, videoName;
	Globals::getFileDirFromQString(filePath, videoPath);
	Globals::getFileNameFromQString(filePath, videoName);
	videoPath = videoPath + tr("analyze\\") + videoName;
	QByteArray ba = videoPath.toLocal8Bit();
	char* path = ba.data();
	videoWriter = cvCreateVideoWriter(path, CV_FOURCC('X', 'V', 'I', 'D'), fps, captureSize, 1);
	int part = eventList.size() / LIMIT;
	if(eventList.size()%LIMIT!=0)
		part++;
	//����һ֡��Ϊ����ͼ��
	double alpha_value = 0.7;
	char eventNumber[128];
	CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0.5, 1, CV_AA);
	IplImage* allEventImage = cvCreateImage(captureSize, 8, 3);
	//����Ƶ�ֳ�part�ˣ��ֱ��û�ν��кϳɱ���
	for(int i = 0; i < part; i++)
	{
		//QTime time;
		//time.start();

		//����ÿһ�ε��±�
		int l_index, r_index;
		if(i != part-1)
		{
			l_index = i*LIMIT;
			r_index = (i+1)*LIMIT;
		}else
		{
			l_index = i*LIMIT;
			r_index = eventList.size();
		}
		//��ÿһ����Ƶ���кϳɱ���    �ǵ�Ҫ����֡���
		/*
		bool* mark = new bool[r_index-l_index];
		for(int j = l_index; j < r_index; j++)
			mark[j] = true;
		*/
		int frameCount = 0;   //��¼֡ƫ����
		int endCount = 0;     //��¼�м����ڵ��Ѿ����
		
		while(endCount < r_index-l_index)
		{
			cvCopy(baseFrame, allEventImage);
			for(int j = l_index; j < r_index; j++)
			{
				int frameNum = eventList[j].startFrame+frameCount*jiange;
				if(frameCount == eventList[j].trackList.size())  //ע������һ���ǡ�==��������ǡ�>=���Ļ���ʹendCount�ظ���Ӽ��Σ���ʧ�ܶ�֡
				{
					endCount++;
					//emit sendProcessBarValue(endCount);
					continue;
				}else if(frameCount > eventList[j].trackList.size())
				{
					continue;
				}
				CvRect rect = eventList[j].trackList[frameCount];
				cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, frameNum);
				frame = cvQueryFrame(capture);
				//��ȡ����ͼ��ϳ�
				cvSetImageROI(frame, rect);
				sprintf(eventNumber, "%d", j);
				cvPutText(frame, eventNumber, cvPoint(10, 15), &font, EventNodeOperation::sampleColor[1]);
				cvSetImageROI(allEventImage, rect);
				cvAddWeighted(frame, alpha_value, allEventImage, 1 - alpha_value, 0, allEventImage);
				cvResetImageROI(allEventImage);  //ȥ����һ���������
				cvResetImageROI(frame);
			}
			for(int j = 0; j < jiange; j++)  //�ظ�д��jiange֡
			{
				cvWriteFrame(videoWriter, allEventImage);
			}
			frameCount++;
		}
		//int time_diff = time.elapsed();
		//QString tr_time_diff = QString::number(int(time_diff/1000), 10);
		//tr_time_diff = tr("100���¼��ϳ�ʱ��Ϊ:") + tr_time_diff;
		//emit sendProcessInfo(tr_time_diff);
	}

	//�ǵ��ͷſռ�
	cvReleaseVideoWriter(&videoWriter);
	cvReleaseImage(&allEventImage);
	//cvReleaseFont
}

void VideoAnalyze::saveEventToFile()
{
	if(isSaveToFile)
	{
		EventNodeOperation::eventFilter(eventList);
		QString fileDir, fileName;
		Globals::getFileDirFromQString(filePath, fileDir);
		Globals::getFileNameFromQString(filePath, fileName);
		//fileDir = fileDir+"\\analyze";
		QDir tempDir(fileDir);
		if(!tempDir.cd("analyze"))
		{
			tempDir.mkdir("analyze");
			tempDir.cd("analyze");
		}
		QString newFilePath = fileDir + "analyze\\" + fileName + ".txt";
		QByteArray ba = newFilePath.toLocal8Bit();
		char* newFilePath_char = ba.data();
		FileOperation::writeToFile(newFilePath_char, jiange, fps, key_jiange, eventList);
	}
}

void VideoAnalyze::getKeyFrameJiange()
{
	//CvInvoke.cvQueryFrame(capture);//2.4�汾����������ܽ���cvSetCaptureProperty����

	int framePosition = 0;
	cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, 7);
	//cvQueryFrame(capture);
	framePosition = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);
	int index1 = framePosition;
	cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, 5);
	cvQueryFrame(capture);
	framePosition = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);
	int index2 = framePosition;
	key_jiange = index2 - index1;
}

void VideoAnalyze::drawAbstracts()
{
	if(!capture)
	{
		QByteArray ba = filePath.toLocal8Bit();
		const char *file = ba.data();
		capture = cvCaptureFromAVI(file);
		CvSize captureSize = cvSize((int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
		(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
		fps = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
		frameCount = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);

		qImg = new QImage(QSize(captureSize.width,captureSize.height), QImage::Format_RGB888);
		iplImg = cvCreateImageHeader(captureSize,  8, 3);
		iplImg->imageData = (char*)qImg->bits();
	}
	if(isSaveToFile)
	{
		IplImage* frame;
		for(int i = 0; i < eventList.size(); i++)
		{
			EventNode node = eventList[i];
			Rect rect = node.trackList[i];
			cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, node.startFrame);
			//int framePosition = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);
			frame = cvQueryFrame(capture);
			cvRectangle(frame, cvPoint(rect.x, rect.y), cvPoint(rect.x+rect.width, rect.y+rect.height), cvScalar(255, 0, 0));
			if(frame)  
			{  
				if (frame->origin == IPL_ORIGIN_TL)  
				{  
					cvCopy(frame,iplImg,0);  
				}  
				else  
				{  
					cvFlip(frame,iplImg,0);  
				}  
				cvCvtColor(iplImg,iplImg,CV_BGR2RGB);
			}
			QString startTime = tr("��ʼʱ��:") + Globals::getTimeFromFrameNum(node.startFrame, fps);
			QString endTime = tr("����ʱ��:") + Globals::getTimeFromFrameNum(node.endFrame, fps);
			emit sendDrawAbstracts(*qImg, startTime, endTime);
			msleep(100);
		}
	}



	//�����źŻ�ժҪ�¼�����ͼ
		//emit sendDrawAbstracts();
}

void VideoAnalyze::getBaseFrame()
{
	QByteArray ba = filePath.toLocal8Bit();
	const char *file = ba.data();
	capture = cvCaptureFromAVI(file);
	CvCapture*tempCapture = cvCaptureFromAVI(file);
	captureSize = cvSize((int)cvGetCaptureProperty(tempCapture, CV_CAP_PROP_FRAME_WIDTH),
			(int)cvGetCaptureProperty(tempCapture, CV_CAP_PROP_FRAME_HEIGHT));
	baseFrame = cvCreateImage(captureSize, 8, 3);
	IplImage* tempFrame = 0;
	int frameNum = 0;
	if(tempCapture)
	{
		while((tempFrame=cvQueryFrame(tempCapture)))
		{
			if(frameNum == 5)
			{
				cvCopy(tempFrame, baseFrame);
				break;
			}
			frameNum++;
		}
	}
	cvReleaseCapture(&tempCapture);
}

VideoAnalyze::VideoAnalyze(QObject* parent = 0):QThread(parent)
{
	//this->filePath = filePath;
	fps = 0;
	minArea = 1000;
	maxArea = 60000;
	jiange = 2;
	key_jiange = 0;
	maxEventNum = 0;
	minEventNum = 10000000;
	LIMIT = 100;
	isContinue = true;
	isShowVideo = false;
	isSaveToFile = false;
	isReadFromFile = false;
	isBatch = false;
	isIgnoreExistAnalyze = false;
	captureSize = cvSize(0,0);

	qImg = 0;
	iplImg = 0;
	baseFrame = 0;
	capture = 0;
	videoWriter = 0;
}

VideoAnalyze::VideoAnalyze(void)
{
}


VideoAnalyze::~VideoAnalyze(void)
{
}

#include "VideoAnalyze.h"

int VideoAnalyze::SELECTEVENT = 4;

void VideoAnalyze::run()
{
	if(flag == VideoAnalyze::SELECTEVENT)
	{
		videoSearch();
		return;
	}

	isContinue = true;
	if(isRealTime)
	{
		realTimeAnalysis();
	}else if(isBatch)
	{
		this->batchAnalysis();
	}else
	{
		this->singleAnalysis();
	}
	isContinue = false;
	emit sendAnalyzeButtonState(0);
	emit sendEndTimeCount();
	emit sendEventCount(eventList.size());
}

//��Ƶ���ݼ���
void VideoAnalyze::videoSearch()
{
	//FILE*fs = fopen("output.txt", "w+");
	//fprintf(fs, "13456%s\n", data.toLocal8Bit().data());
	//fclose(fs);
	//��һ����ɾ��ԭ�����е�ժҪ�¼�
	emit sendRemoveAllAbstracts();
	//�ڶ���������data����
	QStringList dataList = data.split("|");
	//1����ʱ������
	int isTimeLimit = 0;
	{
		QStringList subDataList = dataList[0].split(",");
		if(subDataList[0].compare("1")==0)
		{
			isTimeLimit = 1;
		}
	}
	//2�����˶�����
	int isMotionLimit = 0;
	{
		if(dataList[1].compare("1")==0)
		{
			isMotionLimit = 1;
		}
	}
	//3������ɫ����
	int isColorLimit = 0;
	int r=0,g=0,b=0;
	{
		QStringList subDataList = dataList[2].split(",");
		if(subDataList[0].compare("1")==0)
		{
			isColorLimit = 1;
			r = subDataList[1].toInt();
			g = subDataList[2].toInt();
			b = subDataList[3].toInt();
		}
	}
	//4�����������
	int isTypeLimit = 0;
	int type=0;
	{
		QStringList subDataList = dataList[3].split(",");
		if(subDataList[0].compare("1")==0)
		{
			isTypeLimit = 1;
			type = subDataList[1].toInt();
		}
	}
	//����������������ժҪ����ժҪ����ɸѡ������Ҫ�½�һ���ռ�洢ɸѡ��ժҪ��
	//��ʼ��
	if(!capture)
	{
		QByteArray ba = filePath.toLocal8Bit();
		const char *file = ba.data();
		capture = cvCaptureFromAVI(file);
		captureSize = cvSize((int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
			(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
		fps = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
		frameCount = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
		qImg = new QImage(QSize(captureSize.width,captureSize.height), QImage::Format_RGB888);
		iplImg = cvCreateImageHeader(captureSize,  8, 3);
		iplImg->imageData = (char*)qImg->bits();
		getBaseFrame();
	}

	if(isTimeLimit)//ʱ��ɸѡ
	{
	}
	if(isMotionLimit)//�˶�����ɸѡ
	{
		isSaveToFile = true;
		QString fileDir, fileName;
		Globals::getFileDirFromQString(filePath, fileDir);
		Globals::getFileNameFromQString(filePath, fileName);
		fileDir = fileDir + tr("analyze\\");
		QString analyzeFilePath = fileDir + fileName + tr(".txt");
		FileOperation::readFromFile(analyzeFilePath, jiange, fps, key_jiange, eventList);
		EventNodeOperation::selectAbstractEvent(eventList, lineP1, lineP2, rectP1, rectP2);
	}
	if(isColorLimit)//��ɫɸѡ
	{
	}
	if(isTypeLimit)//���ɸѡ
	{
		typeFilter(type);
	}
	
	//���Ĳ����ػ�ժҪ
	drawAbstracts();
	release();
}

//������
void VideoAnalyze::typeFilter(int type)
{
	CascadeClassifier car_detector, human_detector;
	if( !car_detector.load( "car_detector.xml" ) || !human_detector.load("human_detector.xml")) 
	{
		printf("--(!)Error loading\n"); 
		return ; 
	}

	for(vector<EventNode>::iterator iter=eventList.begin(); iter!=eventList.end(); )
	{
		bool mark = false;
		if(type == 0)  //��
		{
			mark = isHumanEvent((*iter), human_detector);
		}else if(type == 1) //��
		{
			mark = isCarEvent((*iter), car_detector);
		}else if(type == 2) //����
		{
			mark = !isCarEvent((*iter), car_detector) && !isHumanEvent((*iter), human_detector);
		}
		if(!mark)  //ɾ�����Ǽ����Ķ���
			iter = eventList.erase(iter);
		else
			iter++ ;
	}
	
}

//�ж��¼��ǲ��������¼�
bool VideoAnalyze::isHumanEvent(EventNode node, CascadeClassifier human_detector)
{
	int frameNum = node.startFrame+node.trackList.size()/3*jiange;
	//int frameNum = (*iter).startFrame;
	cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, frameNum);
	int count = 0;
	for(int i = frameNum; i < node.endFrame; i++)//���֡
	{
		frame = cvQueryFrame(capture);
		if(i%jiange==0)
		{
			std::vector<Rect> detectRects;
			Mat fImage(frame,0);
			//car_detector.detectMultiScale(fImage, detectRects, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, Size(100, 100), Size(1000,1000) );
			human_detector.detectMultiScale(fImage, detectRects, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, Size(10, 10), Size(100,100) );
			for(int j = 0; j < detectRects.size(); j++)
			{
				if(EventNodeOperation::isTheSame(detectRects[j], node.trackList[frameNum-node.startFrame]))
				{
					count++;
					break;
				}
			}
			if(count >=3)
				return true;
			frameNum++;
		}
	}
	return false;
}

//�ж��ǲ��ǳ����¼�
bool VideoAnalyze::isCarEvent(EventNode node, CascadeClassifier car_detector)
{
	int frameNum = node.startFrame+node.trackList.size()/3*jiange;
	//int frameNum = (*iter).startFrame;
	cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, frameNum);
	int count = 0;
	for(int i = frameNum; i < node.endFrame; i++)//���֡
	{
		frame = cvQueryFrame(capture);
		if(i%jiange==0)
		{
			std::vector<Rect> detectRects;
			Mat fImage(frame,0);
			car_detector.detectMultiScale(fImage, detectRects, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, Size(100, 100), Size(1000,1000) );
			//human_detector.detectMultiScale(fImage, detectRects, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, Size(10, 10), Size(100,100) );
			for(int j = 0; j < detectRects.size(); j++)
			{
				if(EventNodeOperation::isTheSame(detectRects[j], node.trackList[frameNum-node.startFrame]))
				{
					count++;
					break;
				}
			}
			if(count >=3)
				return true;
			frameNum++;
		}
	}
	return false;
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
    cvFindContours( dst, stor, &cont, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
	//cvFindContours( dst, stor, &cont, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));
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

	//EventNodeOperation::bianliEventList(eventList, frameNum);
	int count = EventNodeOperation::bianliEventList(eventList, frameNum, fps);
	emit sendEventCount(count);

	cvReleaseMemStorage(&stor);
	cvReleaseImage(&pyr);
}

void VideoAnalyze::singleAnalysis()
{
	if(!capture)
		init();
	if(!isReadFromFile)
	{
		isSaveToFile = false;
		QString fileDir, fileName, info;
		Globals::getFileDirFromQString(filePath, fileDir);
		Globals::getFileNameFromQString(filePath, fileName);
		info = tr("���ڷ�����Ƶ:") + fileName;
		emit sendProcessInfo(info);
		this->analyzeVideo();
		this->saveEventToFile();
		info = tr("��������ȫժҪ�б�");
		emit sendProcessInfo(info);
		this->drawAbstracts();
		info = tr("��������ȫժҪ��Ƶ");
		emit sendProcessInfo(info);
		this->createAllEventVideo();
	}else
	{
		isSaveToFile = true;
		//this->getBaseFrame();
		QString fileDir, fileName;
		Globals::getFileDirFromQString(filePath, fileDir);
		Globals::getFileNameFromQString(filePath, fileName);
		fileDir = fileDir + tr("analyze\\");
		QString analyzeFilePath = fileDir + fileName + tr(".txt");
		FileOperation::readFromFile(analyzeFilePath, jiange, fps, key_jiange, eventList);
		this->drawAbstracts();
		//this->createAllEventVideo();//���Դ���
	}
	release();
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

		if(!capture)
			init();
		if(!isReadFromFile)
		{
			isSaveToFile = false;
			this->analyzeVideo();
			this->saveEventToFile();
			this->createAllEventVideo();
		}else
		{
			isSaveToFile = true;
		}

		release();
	}
}

void VideoAnalyze::realTimeAnalysis()
{
	//��ʼ������ͷ��Ƶ
	initRealTime();
	analyzeRealTimeVideo();
	filePath = tr("videowrite.avi"); //������д���ı��ļ�֮ǰ����
	//emit sendProcessInfo(tr("��ʼд���ļ�"));
	this->saveEventToFile();
	release();
	init();
	//emit sendProcessInfo(tr("��ʼ��������ժҪ��Ƶ"));
	this->createAllEventVideo();
	release();
}

void VideoAnalyze::analyzeRealTimeVideo()
{
	int N = 3;
	IplImage*motion = 0;   //�ڴ�δ�ͷţ�
	IplImage**buf = 0;
	IplImage*mhi = 0;
	int last = 0;
	double lastTime = 0;
	if(capture)
	{
		//����һ��д��Ƶ����
		CvVideoWriter*camWriter= cvCreateVideoWriter("videowrite.avi", CV_FOURCC('X', 'V', 'I', 'D'), fps, captureSize, 1);

		mhi = cvCreateImage(captureSize, IPL_DEPTH_32F, 1);
		cvZero(mhi);
		buf = new IplImage*[N];
		for (int i = 0; i < N; i++)
		{
			//cvReleaseImage(&buf[i]);
			buf[i] = cvCreateImage(captureSize, IPL_DEPTH_8U, 1);
			cvZero(buf[i]);
		}
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
			cvWriteFrame(camWriter, frame);
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
					int value = 50;
					if(isShowVideo)
					{
						emit sendQImage(*qImg, value);
						emit sendQImage2(*qImg);
						msleep(10);
					}else
					{
						emit sendProcessBarValue(value);
					}
				}
			}
			frameNum++;
		}

		cvReleaseVideoWriter(&camWriter);
	}
	else
	{
		emit sendOpenFileFailed();
	}
}

void VideoAnalyze::analyzeVideo()
{
	int N = 3;
	IplImage*motion = 0;   //�ڴ�δ�ͷţ�
	IplImage**buf = 0;
	IplImage*mhi = 0;
	int last = 0;
	double lastTime = 0;
	if(capture)
	{
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
		//int64 start_time = cv::getTickCount();
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
		//int64 end_time = cv::getTickCount();
		/*
		FILE*fs = fopen("output.txt", "w+");
		fprintf(fs, "%d %d %d %d\n", captureSize.width, captureSize.height, fps, jiange);
		fprintf(fs, "frameCount=%d\n", frameNum);
		fprintf(fs, "time=%f\n", (end_time-start_time)/cv::getTickFrequency());
		fclose(fs);
		*/
		//printf("frameCount=%d\n", frameNum);
		//printf("time=%f\n", (end_time-start_time)/cv::getTickFrequency()/1000);
	}
	else
	{
		emit sendOpenFileFailed();
	}
}

void VideoAnalyze::createAllEventVideo()
{
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
				//emit sendProcessInfo(tr("����")+QString::number(2, 10));
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
				//emit sendProcessInfo(tr("����")+QString::number(frameNum, 200));
				//msleep(200);
				cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, frameNum);
				frame = cvQueryFrame(capture);
				//emit sendProcessInfo(tr("����")+QString::number(frame->height, 10));
				//msleep(200);
				//��ȡ����ͼ��ϳ�
				cvSetImageROI(frame, rect);
				sprintf(eventNumber, "%d", j+1);
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
		EventNodeOperation::eventFilter(eventList, fps);
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
		//emit sendProcessInfo(newFilePath_char);
		msleep(1000);
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
{/*
	for(int i = 0; i < 19; i++)
	{
		frame = cvQueryFrame(capture);
	}*/
	int framePosition;
	if(isSaveToFile)
	{
		for(int i = 0; i < eventList.size(); i++)
			//for(int i = 0; i < 30; i++)
		{
			EventNode node = eventList[i];
			Rect rect = node.trackList[node.trackList.size()/2];
			cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, node.startFrame+node.trackList.size()/2*jiange);
			//framePosition = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);
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
			emit sendDrawAbstracts(*qImg, startTime, endTime, i);
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

IplImage* VideoAnalyze::getFrameByNumber(int pos)
{
	QByteArray ba = filePath.toLocal8Bit();
	const char *file = ba.data();
	CvCapture*tempCapture = cvCaptureFromAVI(file);
	captureSize = cvSize((int)cvGetCaptureProperty(tempCapture, CV_CAP_PROP_FRAME_WIDTH),
			(int)cvGetCaptureProperty(tempCapture, CV_CAP_PROP_FRAME_HEIGHT));
	IplImage* tempFrame = cvCreateImage(captureSize, 8, 3);
	int frameNum = 0;
	if(tempCapture)
	{
		cvSetCaptureProperty(tempCapture, CV_CAP_PROP_POS_FRAMES, pos);
		frame = cvQueryFrame(tempCapture);
		cvCopy(frame, tempFrame);
	}
	cvReleaseCapture(&tempCapture);
	return tempFrame;
}

bool VideoAnalyze::initRealTime()
{
	capture = cvCaptureFromCAM(-1);
	if(!capture)
		return false;
	captureSize = cvSize((int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
			(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
	fps = 15;   //����ʵʱ����֡��Ϊ15
	qImg = new QImage(QSize(captureSize.width,captureSize.height), QImage::Format_RGB888);
	iplImg = cvCreateImageHeader(captureSize,  8, 3);
	iplImg->imageData = (char*)qImg->bits();
	//��ȡ����֡
	baseFrame = cvCreateImage(captureSize, 8, 3);
	for(int i = 0; i <= 5 && (frame=cvQueryFrame(capture)); i++)
	{
		if(i == 5)
		{
			cvCopy(frame, baseFrame);
			break;
		}
	}
	minArea = 1000;
	maxArea = 1000000;
	eventList.clear();
	return true;
}

bool VideoAnalyze::init()
{
	QByteArray ba = filePath.toLocal8Bit();
	const char *file = ba.data();
	capture = cvCaptureFromAVI(file);
	if(!capture)
		return false;
	captureSize = cvSize((int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
			(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
	fps = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	frameCount = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	qImg = new QImage(QSize(captureSize.width,captureSize.height), QImage::Format_RGB888);
	iplImg = cvCreateImageHeader(captureSize,  8, 3);
	iplImg->imageData = (char*)qImg->bits();
	getBaseFrame();
	minArea = 1000;
	maxArea = 1000000;
	eventList.clear();
	return true;
}

void VideoAnalyze::release()
{
	if(capture)
	{
		cvReleaseCapture(&capture);
		capture = 0;
	}
	if(qImg)
	{
		delete qImg;
		qImg = 0;
	}
	if(iplImg)   //������ܻ������ΪQImage��IplImage�ǰ󶨵ģ��������öϵ����
	{
		cvReleaseImage(&iplImg);
		iplImg = 0;
	}
	if(baseFrame)
	{
		cvReleaseImage(&baseFrame);
		baseFrame = 0;
	}

	flag = -1;

	lineP1.x = -1;
	lineP1.y = -1;
	lineP2.x = -1;
	lineP2.y = -1;
	rectP1.x = -1;
	rectP1.y = -1;
	rectP2.x = -1;
	rectP2.y = -1;
}

void VideoAnalyze::getSettingData(int zoom,int width,int height,int min_area,int max_area,int jiange,int fps,int max_event_num)
{
	this->minArea = min_area;
	this->maxArea = max_area;
	this->jiange = jiange;
	this->LIMIT = max_event_num;

	//fps = data.fps;
	//int zoom = data.zoom;
}

void VideoAnalyze::getShuaixuanData(Point currentLineP1,Point currentLineP2,Point currentRectP1,Point currentRectP2,QString data)
{
	lineP1.x = currentLineP1.x;
	lineP1.y = currentLineP1.y;
	lineP2.x = currentLineP2.x;
	lineP2.y = currentLineP2.y;

	rectP1.x = currentRectP1.x;
	rectP1.y = currentRectP1.y;
	rectP2.x = currentRectP2.x;
	rectP2.y = currentRectP2.y;
	
	this->data = data;
	//��ʼɸѡ����
	flag = VideoAnalyze::SELECTEVENT;
	start();
}

VideoAnalyze::VideoAnalyze(QObject* parent = 0):QThread(parent)
{
	fps = 0;
	minArea = 1000;
	maxArea = 100000;
	jiange = 2;
	key_jiange = 0;
	maxEventNum = 0;
	minEventNum = 10000000;
	LIMIT = 100;
	isContinue = false;
	isShowVideo = false;
	isSaveToFile = false;
	isReadFromFile = false;
	isBatch = false;
	isRealTime = false;
	isIgnoreExistAnalyze = false;
	captureSize = cvSize(0,0);

	qImg = 0;
	iplImg = 0;
	baseFrame = 0;
	capture = 0;
	videoWriter = 0;

	lineP1.x = -1;
	lineP1.y = -1;
	lineP2.x = -1;
	lineP2.y = -1;
	rectP1.x = -1;
	rectP1.y = -1;
	rectP2.x = -1;
	rectP2.y = -1;
}

VideoAnalyze::VideoAnalyze(void)
{
}


VideoAnalyze::~VideoAnalyze(void)
{
}

void VideoAnalyze::updateOneSecond()
{
}
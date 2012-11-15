#include "realtimethread.h"

//public functions
//�߳����ʼ������
bool RealTimeThread::init()
{
	capture = cvCaptureFromCAM(num);
	if(!capture)
		return false;
	captureSize = cvSize((int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
			(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
	qImg = new QImage(QSize(captureSize.width,captureSize.height), QImage::Format_RGB888);
	iplImg = cvCreateImageHeader(captureSize,  8, 3);
	iplImg->imageData = (char*)qImg->bits();
	return true;
}

//�߳����ͷź���
void RealTimeThread::release()
{
	//msleep(100);
	while(this->isRunning())
	{
		msleep(10);
	}
	if(capture)
		cvReleaseCapture(&capture);
	if(writer)
		cvReleaseVideoWriter(&writer);
	if(iplImg)
		cvReleaseImage(&iplImg);
}

//��������ͷ���
void RealTimeThread::setNum(int num)
{
	this->num = num;
}

//��ʼ���
void RealTimeThread::startMonitor()
{
	//��ȡ����ʱ�䣬��Ϊ��Ƶ�ļ���
	QString time = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_");
	QString filePath = fileDir + time + QString::number(num+1) + tr(".avi");
	QByteArray ba = filePath.toLocal8Bit();
	char* filePath_c = ba.data();
	//ע���ַ���������Ƶ�ļ�����·�������������ģ���������
	writer = cvCreateVideoWriter(filePath_c, CV_FOURCC('X', 'V', 'I', 'D'), 25, captureSize, 1);
	eventList.clear();
	isSaveToFile = true;
}

//ֹͣ���
void RealTimeThread::endMonitor()
{
	isSaveToFile = false;
	msleep(100);
	cvReleaseVideoWriter(&writer);
	writer = 0;
}

//������Ƶ���ļ�
void RealTimeThread::saveToFile()
{
	if(!isSaveToFile || !writer)
	{
		return;
	}
	cvWriteFrame(writer, frame);
}

//��ʼ����ʾ֡��ģ��
void RealTimeThread::initShowFPS()
{
	realTimeFps = 0;
	frameCountInSecond = 0;
	cvInitFont(&fontFPS, CV_FONT_HERSHEY_SIMPLEX, 1, 1, 1, 1, CV_AA);
}

//��ʾ֡��
void RealTimeThread::showFPS()
{
	if(!isShowFPS)
		return;
	char eventNumber[128];
	sprintf(eventNumber, "fps=%d", realTimeFps);
	cvPutText(frame, eventNumber, cvPoint(10, 30), &fontFPS, cvScalar(255,0,0));
	frameCountInSecond++;
}

//�ͷ���ʾ֡��ģ��
void RealTimeThread::releseShowFPS()
{
}

void RealTimeThread::setDataFromSetting(QString data)
{
	QStringList dataList = data.split('|');
	scale = dataList.at(0).toInt();   //��Ƶ����
	min_area = dataList.at(1).toInt();   //��С���
	max_area = dataList.at(2).toInt();  //������
	jiange = dataList.at(3).toInt();    //����֡���
	fps = dataList.at(4).toInt();     //¼��֡��
	max_event_num = dataList.at(5).toInt(); //ͬһʱ�����ժҪ��
	fileDir = dataList.at(6);  //������ļ���·��
}

//���캯��
RealTimeThread::RealTimeThread(QObject *parent)
	: QThread(parent)
{
	capture = 0;
	writer = 0;
	iplImg = 0;
    frame = 0;
    qImg = 0;
	fileDir = QString("D:\\realTimeVideo\\");
	isShowFPS = true;
	isSaveToFile = false;
	if(isShowFPS)
		initShowFPS();  //��ʼ����ʾ֡��
}

RealTimeThread::~RealTimeThread()
{

}

void RealTimeThread::update_mhi(IplImage*&img, IplImage*&dst, int frameNum, IplImage**&buf, int&last, IplImage*&mhi, CvSize size, double&lastTime)
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
		if (r.height * r.width > min_area && r.height * r.width < max_area)
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

//protected functions
void RealTimeThread::run()
{	
	init();
	//�㷨��ʼ
	int N = 3;
	IplImage*motion = 0;   //�ڴ�δ�ͷţ�
	IplImage**buf = 0;
	IplImage*mhi = 0;
	int last = 0;
	double lastTime = 0;
	flag = 1;
	int frameNum = 0;

	mhi = cvCreateImage(captureSize, IPL_DEPTH_32F, 1);
	cvZero(mhi);
	buf = new IplImage*[N];
	for (int i = 0; i < N; i++)
	{
		//cvReleaseImage(&buf[i]);
		buf[i] = cvCreateImage(captureSize, IPL_DEPTH_8U, 1);
		cvZero(buf[i]);
	}
	for(; flag; )
	{
		frame = cvQueryFrame(capture);  //��ȡ֡

		//�㷨����
		if (!frame || !flag)
		{
			msleep(100);//�����������ͣ�Ļ�������Ϊ��ʵͼƬ���ͷŶ������ڴ����
			break;
		}

		if(isSaveToFile)
		{
			saveToFile();    //�����ļ�
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
			}
		}
		//�㷨���ֽ���
		showFPS();       //��ʾ֡��
		if (frame->origin == IPL_ORIGIN_TL)  
		{  
			cvCopy(frame,iplImg,0);  
		}  
		else  
		{  
			cvFlip(frame,iplImg,0);  
		}
		cvCvtColor(iplImg,iplImg,CV_BGR2RGB);
		emit sendCameraImage(num, *qImg);
		//msleep(20);
	}
}

//ʱ�ӵ����slot����
void RealTimeThread::timerTimeOut()
{
	realTimeFps = frameCountInSecond;
	frameCountInSecond = 0;
}
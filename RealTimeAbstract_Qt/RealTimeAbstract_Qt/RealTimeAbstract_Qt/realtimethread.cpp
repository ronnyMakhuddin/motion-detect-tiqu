#include "realtimethread.h"

//public functions
//线程类初始化函数
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

//线程类释放函数
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

//设置摄像头编号
void RealTimeThread::setNum(int num)
{
	this->num = num;
}

//开始监控
void RealTimeThread::startMonitor()
{
	//获取本地时间，作为视频文件名
	QString time = QDateTime::currentDateTime().toString("yyyy_MM_dd_hh_mm_ss_");
	QString filePath = fileDir + time + QString::number(num+1) + tr(".avi");
	QByteArray ba = filePath.toLocal8Bit();
	char* filePath_c = ba.data();
	//注意字符串处理，视频文件名和路径名不能有中文，否则会出错
	writer = cvCreateVideoWriter(filePath_c, CV_FOURCC('X', 'V', 'I', 'D'), 25, captureSize, 1);
	eventList.clear();
	isSaveToFile = true;
}

//停止监控
void RealTimeThread::endMonitor()
{
	isSaveToFile = false;
	msleep(100);
	cvReleaseVideoWriter(&writer);
	writer = 0;
}

//保存视频到文件
void RealTimeThread::saveToFile()
{
	if(!isSaveToFile || !writer)
	{
		return;
	}
	cvWriteFrame(writer, frame);
}

//初始化显示帧率模块
void RealTimeThread::initShowFPS()
{
	realTimeFps = 0;
	frameCountInSecond = 0;
	cvInitFont(&fontFPS, CV_FONT_HERSHEY_SIMPLEX, 1, 1, 1, 1, CV_AA);
}

//显示帧率
void RealTimeThread::showFPS()
{
	if(!isShowFPS)
		return;
	char eventNumber[128];
	sprintf(eventNumber, "fps=%d", realTimeFps);
	cvPutText(frame, eventNumber, cvPoint(10, 30), &fontFPS, cvScalar(255,0,0));
	frameCountInSecond++;
}

//释放显示帧率模块
void RealTimeThread::releseShowFPS()
{
}

void RealTimeThread::setDataFromSetting(QString data)
{
	QStringList dataList = data.split('|');
	scale = dataList.at(0).toInt();   //视频缩放
	min_area = dataList.at(1).toInt();   //最小面积
	max_area = dataList.at(2).toInt();  //最大面积
	jiange = dataList.at(3).toInt();    //分析帧间隔
	fps = dataList.at(4).toInt();     //录制帧率
	max_event_num = dataList.at(5).toInt(); //同一时间最大摘要数
	fileDir = dataList.at(6);  //保存的文件夹路径
}

//构造函数
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
		initShowFPS();  //初始化显示帧率
}

RealTimeThread::~RealTimeThread()
{

}

void RealTimeThread::update_mhi(IplImage*&img, IplImage*&dst, int frameNum, IplImage**&buf, int&last, IplImage*&mhi, CvSize size, double&lastTime)
{
	int N = 3;
	double MHI_DURATION = 0.5;//0.5s为运动跟踪的最大持续时间
	double timestamp = clock()/100.0; // get current time in seconds 时间戳
    int i, idx1, idx2;
    IplImage* silh;
    IplImage* pyr = cvCreateImage( cvSize((size.width & -2)/2, (size.height & -2)/2), 8, 1 );
    CvMemStorage *stor;
    CvSeq *cont;

    //先进行数据的初始化
    if( !mhi || mhi->width != size.width || mhi->height != size.height )
    {
		if( buf == 0 ) //若尚没有初始化则分配内存给他
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
    /*将当前要处理的帧转化为灰度放到buffer的最后一帧中*/
    cvCvtColor( img, buf[last], CV_BGR2GRAY ); // convert frame to grayscale

    idx1 = last;
    idx2 = (last + 1) % N; // index of (last - (N-1))th frame
    last = idx2;
    // 做帧差
    silh = buf[idx2];//差值的指向idx2 |idx2-idx1|-->idx2(<-silh)
    cvAbsDiff( buf[idx1], buf[idx2], silh ); // get difference between frames
    
    // 对差图像做二值化
    cvThreshold( silh, silh, 30, 255, CV_THRESH_BINARY ); //threshold it,二值化
    
    cvUpdateMotionHistory( silh, mhi, timestamp, MHI_DURATION ); // update MHI
 
    cvConvert( mhi, dst );//将mhi转化为dst,dst=mhi   
    
    // 中值滤波，消除小的噪声
    cvSmooth( dst, dst, CV_MEDIAN, 3, 0, 0, 0 );
    
    
    cvPyrDown( dst, pyr, CV_GAUSSIAN_5x5 );// 向下采样，去掉噪声，图像是原图像的四分之一
    cvDilate( pyr, pyr, 0, 1 );  // 做膨胀操作，消除目标的不连续空洞
    cvPyrUp( pyr, dst, CV_GAUSSIAN_5x5 );// 向上采样，恢复图像，图像是原图像的四倍
	
    //
    // 下面的程序段用来找到轮廓
    //
    // Create dynamic structure and sequence.
    stor = cvCreateMemStorage(0);
    cont = cvCreateSeq(CV_SEQ_ELTYPE_POINT, sizeof(CvSeq), sizeof(CvPoint) , stor);
    
    // 找到所有轮廓
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
	//算法开始
	int N = 3;
	IplImage*motion = 0;   //内存未释放！
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
		frame = cvQueryFrame(capture);  //获取帧

		//算法部分
		if (!frame || !flag)
		{
			msleep(100);//这里如果不暂停的话，会因为现实图片被释放而出现内存错误
			break;
		}

		if(isSaveToFile)
		{
			saveToFile();    //保存文件
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
		//算法部分结束
		showFPS();       //显示帧率
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

//时钟到点的slot函数
void RealTimeThread::timerTimeOut()
{
	realTimeFps = frameCountInSecond;
	frameCountInSecond = 0;
}
#include "VideoAnalyze.h"

void VideoAnalyze::run()
{
	
	this->analyzeVideo();
}

void VideoAnalyze::update_mhi(IplImage*&img, IplImage*&dst, int frameNum, IplImage**&buf, int&last, IplImage*&mhi, CvSize size, double&lastTime)
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

void VideoAnalyze::analyzeVideo()
{
	int N = 3;
	QByteArray ba = filePath.toLocal8Bit();
	const char *file = ba.data();
	capture = cvCaptureFromAVI(file);
	IplImage*motion = 0;   //内存未释放！
	IplImage**buf = 0;
	IplImage*mhi = 0;
	int last = 0;
	double lastTime = 0;
	if(capture)
	{
		CvSize captureSize = cvSize((int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
			(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
		fps = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
		frameCount = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);

		qImg = new QImage(QSize(captureSize.width,captureSize.height), QImage::Format_RGB888);
		iplImg = cvCreateImageHeader(captureSize,  8, 3);
		iplImg->imageData = (char*)qImg->bits();
		
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
		int frameNum = 0;
		while (true)
		{
			frame = cvQueryFrame(capture);
			if (!frame && isContinue)
			{
				msleep(100);//这里如果不暂停的话，会因为现实图片被释放而出现内存错误
				break;
			}
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

	//keyFrameJiange(filePath);

	if(capture)
		cvReleaseCapture(&capture);
	if(qImg)
		delete qImg;
	if(iplImg)
		cvReleaseImage(&iplImg);
}

VideoAnalyze::VideoAnalyze(QObject* parent = 0):QThread(parent)
{
	//this->filePath = filePath;
	fps = 0;
	minArea = 1000;
	maxArea = 60000;
	jiange = 4;
	key_jiange = 0;
	maxEventNum = 0;
	minEventNum = 10000000;
	LIMIT = 100;
	isContinue = true;
	isShowVideo = false;


	qImg = 0;
	iplImg = 0;
	capture = 0;
}

VideoAnalyze::VideoAnalyze(void)
{
}


VideoAnalyze::~VideoAnalyze(void)
{
}

#include "VideoAnalyze.h"

void VideoAnalyze::run()
{

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
	for (; cont; cont->h_next)
	{
		CvRect r = ((CvContour*)cont)->rect;
		if (r.height * r.width > minArea && r.height * r.width < maxArea)
		{
			CvScalar s;

			EventNode node;
			if (eventList.size() == 0)
			{
				EventNodeOperation::insertEventNode(eventList, r, frameNum);
			}
			else
			{
				node = *EventNodeOperation::searchEventList(eventList, r);
				if(!(&node))
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
	CvCapture*capture = cvCaptureFromAVI(filePath.toUtf8().data());
	IplImage*motion = 0;
	IplImage**buf;
	IplImage*mhi;
	int last = 0;
	double lastTime = 0;
	if (capture)
	{
		CvSize captureSize = cvSize((int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),
			(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
		fps = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
		
		minArea = 100;
		maxArea = 1000000;

		mhi = cvCreateImage(captureSize, IPL_DEPTH_32F, 1);
		cvZero(mhi);
		buf = new IplImage*[N];
		for (int i = 0; i < N; i++)
		{
			cvReleaseImage(&buf[i]);
			buf[i] = cvCreateImage(captureSize, IPL_DEPTH_8U, 1);
			cvZero(buf[i]);
		}

		//cvNamedWindow("analyze");
		int totalFrames = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
		IplImage*frame;
		int frameNum = 0;

		//form.analyzeProgressBarSetMaxValue(totalFrames.ToString());

		while (true)
		{
			//form.analyzeProgressBarSetValue(frameNum.ToString());

			frame = cvQueryFrame(capture);
			if (!frame)
			{
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
				//CvInvoke.cvShowImage("analyze", frame);
				//CvInvoke.cvWaitKey(100);
			}
			frameNum++;
		}
		cvDestroyAllWindows();
	}
	else
	{
		//QMessageBox::warning(this, tr("����"), tr("��Ƶ�ļ��𻵻��ʽ����ȷ���޷��򿪣�"));
	}

	//keyFrameJiange(filePath);
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
}

VideoAnalyze::VideoAnalyze(void)
{
}


VideoAnalyze::~VideoAnalyze(void)
{
}

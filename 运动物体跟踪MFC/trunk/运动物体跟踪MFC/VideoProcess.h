#pragma once
#include "cv.h"
#include <windows.h>
#include <iostream>
#include "highgui.h"
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "HistStruct.h"

#include "�˶��������MFC.h"
#include "�˶��������MFCDlg.h"

using namespace std;

class VideoProcess
{
public:
	VideoProcess(void);
	~VideoProcess(void);
};


// various tracking parameters (in seconds) //���ٵĲ�������λΪ�룩
const double MHI_DURATION = 0.5;//0.5sΪ�˶����ٵ�������ʱ��
const double MAX_TIME_DELTA = 0.5;
const double MIN_TIME_DELTA = 0.05;
const int N = 3;
const int PRE_NUM_FRAME = 4;
const int JIANGE_FRAME = 4;
const int LIMIT = 120;
//
const int CONTOUR_MAX_AERA = 60000;
const int CONTOUR_MIN_AERA = 6000;

static struct HistNode*head;
static CString FilePathName;
//  ������
//  img �C ������Ƶ֡
//  dst �C �����
static void  update_mhi( IplImage* img, IplImage* dst, int frameNum, IplImage**&buf, int &last, IplImage*&mhi)
{
	double timestamp = clock()/100.; // get current time in seconds ʱ���
    CvSize size = cvSize(img->width,img->height);
    // get current frame size���õ���ǰ֡�ĳߴ�
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
    // ֱ��ʹ��CONTOUR�еľ�����������
    for(;cont;cont = cont->h_next)
    {
		CvRect r = ((CvContour*)cont)->rect;
	  
	    if(r.height * r.width > CONTOUR_MIN_AERA&&r.height * r.width < CONTOUR_MAX_AERA) // ���С�ķ���������
	    {
			//��ȡ���ο�ͼ���ֱ��ͼ
		    //CvSize size = {r.width,r.height};
	        //CvMat test;
		    //IplImage*sub_img=cvCreateImage(size,IPL_DEPTH_8U,3);
		    //cvGetImage(cvGetSubRect(img,&test,r),sub_img);
		    //cvShowImage( "dst", sub_img );//��������

			CvScalar s;

			//�������
			HistNode*node;
			if(head==NULL)
			{
				createHead(head, r, frameNum);
				node = head;
			}else
			{
				node = searchHist(head, r);
				if(node == NULL)
				{
					node = insertNode(head, r, frameNum);
				}
			}
			s = sampleColor[node->num%5];

	        cvRectangle( img, cvPoint(r.x,r.y),
		    cvPoint(r.x + r.width, r.y + r.height), s, 1, CV_AA,0);
	    }
    }
	bianliNode(head, frameNum);
    // free memory
    cvReleaseMemStorage(&stor);
    cvReleaseImage( &pyr );
}

static void  update_mhi2( IplImage* img, IplImage* dst, int frameNum, IplImage**&buf, int &last, IplImage*&mhi, CvRect &preRect)
{
	double timestamp = clock()/100.; // get current time in seconds ʱ���
    CvSize size = cvSize(img->width,img->height);
    // get current frame size���õ���ǰ֡�ĳߴ�
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
    // ֱ��ʹ��CONTOUR�еľ�����������
    for(;cont;cont = cont->h_next)
    {
		CvRect r = ((CvContour*)cont)->rect;
	  
	    if(r.height * r.width > CONTOUR_MIN_AERA&&r.height * r.width < CONTOUR_MAX_AERA) // ���С�ķ���������
	    {
			//��ȡ���ο�ͼ���ֱ��ͼ
		    //CvSize size = {r.width,r.height};
	       // CvMat test;
		    //IplImage*sub_img=cvCreateImage(size,IPL_DEPTH_8U,3);
		    //cvGetImage(cvGetSubRect(img,&test,r),sub_img);
		    //cvShowImage( "dst", sub_img );//��������
			if(isTheSame(r, preRect))
			{
				CvScalar s;
				s = sampleColor[0];
				cvRectangle( img, cvPoint(r.x,r.y),
				cvPoint(r.x + r.width, r.y + r.height), s, 1, CV_AA,0);
				preRect = r;
			}
	    }
    }
    // free memory
    cvReleaseMemStorage(&stor);
    cvReleaseImage( &pyr );
}

//��ʾ�����¼�����
static void  update_mhi3( IplImage* &img, IplImage* dst, IplImage**&buf, int &last, IplImage*&mhi, CvRect &preRect)
{
	double timestamp = clock()/100.; // get current time in seconds ʱ���
    CvSize size = cvSize(img->width,img->height);
    // get current frame size���õ���ǰ֡�ĳߴ�
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
    // ֱ��ʹ��CONTOUR�еľ�����������
    for(;cont;cont = cont->h_next)
    {
		CvRect r = ((CvContour*)cont)->rect;
	  
	    if(r.height * r.width > CONTOUR_MIN_AERA&&r.height * r.width < CONTOUR_MAX_AERA) // ���С�ķ���������
	    {
			if(isTheSame(r, preRect))
			{
				preRect = r;
			}
	    }
    }
	//��ȡ���ο�ͼ���ֱ��ͼ
	CvSize pre_size = {preRect.width,preRect.height};
	CvMat test;
	IplImage*sub_img=cvCreateImage(pre_size,IPL_DEPTH_8U,3);
	cvGetImage(cvGetSubRect(img,&test,preRect),sub_img);
    //cvShowImage( "AllEvent", sub_img);
	//cvReleaseImage(&img);
	img = sub_img;

    // free memory
    cvReleaseMemStorage(&stor);
    cvReleaseImage( &pyr );
}

static void process(CString str)
{
	CProgressCtrl* p = (CProgressCtrl*)(AfxGetApp()->m_pMainWnd->GetDlgItem(IDC_PROGRESS1));
    IplImage* motion = 0;
    CvCapture* capture = 0;

	// ring image buffer Ȧ��ͼ�񻺳�
	IplImage **buf = 0;//ָ���ָ��
	int last = 0;
	// temporary images��ʱͼ��
	IplImage *mhi = 0; // MHI: motion history image

	USES_CONVERSION;
	capture = cvCaptureFromAVI(str);
    if( capture )
    {
		int totalFrames = (int) cvGetCaptureProperty(capture,  CV_CAP_PROP_FRAME_COUNT);
        cvNamedWindow( "Motion", 1 );//��������
		//cvNamedWindow( "dst", 1 );//��������
        for(int frameNum = 0;;frameNum++)
        {
			p->SetPos(100*frameNum/totalFrames);
            IplImage* image;
			int t = 0;
            if( !(t=cvGrabFrame( capture )))//��׽һ��
                break;
			if(frameNum%JIANGE_FRAME==0)
			{
				image = cvRetrieveFrame( capture );//ȡ�����֡
				if( image )//��ȡ�����ж�motion�Ƿ�Ϊ��
				{
					//cvFlip(image,image,1);//ͼ��ת
					if( !motion )
					{
						motion = cvCreateImage( cvSize(image->width,image->height), 8, 1 );
						//����motion֡����λ��һͨ��
						cvZero( motion );
						//�����motion
						motion->origin = image->origin;
						//�ڴ�洢��˳���ȡ����֡��ͬ
					}
				}
				int posFrames    = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);
				update_mhi( image, motion, posFrames, buf, last, mhi);//������ʷͼ��
				cvShowImage( "Motion", image );//��ʾ�������ͼ��
				if( cvWaitKey(10) >= 0 )//10ms�а�������˳�
					break;
			}
        }
        cvReleaseCapture( &capture );//�ͷ��豸
        cvDestroyWindow( "Motion" );//���ٴ���
		//cvDestroyWindow( "dst" );//���ٴ���
    }
	return;
} 

//��ʾ�����¼���Ƶ
static void displaySingleEvent(int index)
{
	HistNode* node = head;
	for(int i = 0; i < index; i++)
	{
		node=node->next;
	}

    IplImage* motion = 0;
    CvCapture* capture = 0;

	// ring image buffer Ȧ��ͼ�񻺳�
	IplImage **buf = 0;//ָ���ָ��
	int last = 0;
	// temporary images��ʱͼ��
	IplImage *mhi = 0; // MHI: motion history image

	USES_CONVERSION;
	capture = cvCaptureFromAVI( FilePathName);
	if( capture )
    {
		node->rect = node->startRect;
		//��ǰ��֡
		int startFrame = node->startFrame;
		if((node->startFrame - 4) >= 1)
			startFrame = node->startFrame - 4;
		cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, startFrame);
        cvNamedWindow( "Motion_Event", 1 );//��������
		for(int i = 0;i <= node->endFrame - startFrame;i++)
        {   
			int t;
            IplImage* image;
            if( !(t=cvGrabFrame( capture )))//��׽һ��
                break;
			if(i%PRE_NUM_FRAME==0)
			{
				image = cvRetrieveFrame( capture );//ȡ�����֡
				if( image )//��ȡ�����ж�motion�Ƿ�Ϊ��
				{
					//cvFlip(image,image,1);//ͼ��ת
					if( !motion )
					{
						motion = cvCreateImage( cvSize(image->width,image->height), 8, 1 );
						//����motion֡����λ��һͨ��
						cvZero( motion );
						//�����motion
						motion->origin = image->origin;
						//�ڴ�洢��˳���ȡ����֡��ͬ
					}
				}
				update_mhi2( image, motion, -5, buf, last, mhi, node->rect);//������ʷͼ��
				cvShowImage( "Motion_Event", image );//��ʾ�������ͼ��
				if( cvWaitKey(20) >= 0 )//10ms�а�������˳�
					break;
			}
		}
		cvReleaseCapture( &capture );//�ͷ��豸
		cvDestroyWindow( "Motion_Event" );//���ٴ���
	}
}


static void displaySingleEvent2(int index)
{
	HistNode* node = head;
	for(int i = 0; i < index; i++)
	{
		node=node->next;
	}

	USES_CONVERSION;
	CvCapture* capture = cvCaptureFromAVI( FilePathName);
	if( capture )
    {
		node->eventTempNode = node->eventStart;
		//��ȷ��λ
		cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, node->startFrame-1);
		cvGrabFrame( capture );
		int posFrames = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);
		for(int i = 1; posFrames>node->startFrame; i++)
		{
			cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, node->startFrame-i-1);
			cvGrabFrame( capture );
			posFrames = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);
		}
		while(posFrames<node->startFrame-1)
		{
			cvGrabFrame( capture );
			posFrames = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);
		}
		
        cvNamedWindow( "Motion_Event", 1 );//��������
		for(int i = 0;i <= node->endFrame - node->startFrame;i++)
        {   
			int t;
            IplImage* image;
            if( !(t=cvGrabFrame( capture )))//��׽һ��
                break;
			if(i%JIANGE_FRAME==0 && node->eventTempNode!=NULL)
			{
				image = cvRetrieveFrame( capture );//ȡ�����֡
			    CvScalar s = sampleColor[0];
				cvRectangle( image, cvPoint(node->eventTempNode->rect.x, node->eventTempNode->rect.y),
				cvPoint(node->eventTempNode->rect.x + node->eventTempNode->rect.width, node->eventTempNode->rect.y + node->eventTempNode->rect.height), s, 1, CV_AA,0);
				cvShowImage( "Motion_Event", image );//��ʾ�������ͼ��
				if( cvWaitKey(20) >= 0 )//10ms�а�������˳�
					break;
				int posFrames    = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES);
				node->eventTempNode = node->eventTempNode->next;
			}
		}
		cvReleaseCapture( &capture );//�ͷ��豸
		cvDestroyWindow( "Motion_Event" );//���ٴ���
	}
}

//��ʾ�����¼�
static void displayAllEvent(int total, int maxEvent)
{
	HistNode* node = head;
	CvSize size = cvSize(300,300);
    CvCapture* capture = 0;
	IplImage* backImage;
	USES_CONVERSION;
	capture = cvCaptureFromAVI( FilePathName);
	if(capture)
	{
		cvGrabFrame( capture );
		backImage = cvRetrieveFrame( capture );
		size = cvSize(backImage->width,backImage->height);
	}
	
	cvReleaseCapture( &capture );//�ͷ��豸
	//��ʼ��
	while(node)
	{
		USES_CONVERSION;
		node->capture = cvCaptureFromAVI(FilePathName);

		//��ǰ��֡
		int startFrame = node->startFrame;
		if((node->startFrame - 4) >= 1)
			startFrame = node->startFrame - 4;
		cvSetCaptureProperty(node->capture, CV_CAP_PROP_POS_FRAMES, startFrame);

		node->rect = node->startRect;
		node->motion = 0;
		node->buf = 0;
		node->last = 0;
		node->mhi = 0;
		node = node->next;
	}
	node = head;
    
	CvVideoWriter*writer = cvCreateVideoWriter("allEvent.avi",-1,50, size);

	cvNamedWindow( "AllEvent", 1 );//��������
	IplImage* AllEventImage = cvCreateImage(cvGetSize(backImage),backImage->depth , backImage->nChannels);
	cvCopy(backImage,AllEventImage,NULL);
	for(int i = 0; i < maxEvent + 4; i++)
	{
		if(i%JIANGE_FRAME==0)
		{
			IplImage *tempRelease = AllEventImage;
			AllEventImage = cvCreateImage(cvGetSize(backImage),backImage->depth , backImage->nChannels);
			cvCopy(backImage,AllEventImage,NULL);
			cvReleaseImage(&tempRelease);
		}
		int limit = 0;
		while(node)
		{
			if(limit++>=LIMIT)
				break;
			//if(i >= node->endFrame - node->startFrame + 4)
				//continue;
			if(node->capture)
			{
				IplImage* image;
				if( !(cvGrabFrame(node->capture )))//��׽һ��
					break;
				if(i%JIANGE_FRAME==0)
				{
					image = cvRetrieveFrame( node->capture );//ȡ�����֡
					if( image )//��ȡ�����ж�motion�Ƿ�Ϊ��
					{
						//cvFlip(image,image,1);//ͼ��ת
						if( !node->motion )
						{
							node->motion = cvCreateImage( cvSize(image->width,image->height), 8, 1 );
							//����motion֡����λ��һͨ��
							cvZero( node->motion );
							//�����motion
							node->motion->origin = image->origin;
							//�ڴ�洢��˳���ȡ����֡��ͬ
						}
						update_mhi3( image, node->motion, node->buf, node->last, node->mhi, node->rect);//������ʷͼ��
						// Set the image ROI to display the current image
						cvSetImageROI(AllEventImage,node->rect);
						// Resize the input image and copy the it to the Single Big Image
						cvResize(image, AllEventImage);
						// Reset the ROI in order to display the next image
						cvResetImageROI(AllEventImage);
						
					}
				}
			}
			node = node->next;
		}
		node = head;

		cvWriteToAVI( writer, AllEventImage);

		if(i%JIANGE_FRAME==0)
			cvShowImage( "AllEvent", AllEventImage );//��ʾ�������ͼ��
		if( cvWaitKey(10) >= 0 )//10ms�а�������˳�
			break;
	}
	node = head;
	while(node)
	{
		cvReleaseCapture( &node->capture );//�ͷ��豸
		node = node->next;
	}
	cvDestroyWindow( "AllEvent" );//���ٴ���
	cvReleaseVideoWriter( &writer );
}


//��ʾ�����¼�
static void displayAllEvent2(int total, int maxEvent)
{
	HistNode* node = head;
	CvSize size = cvSize(300,300);
    CvCapture* capture = cvCaptureFromAVI( FilePathName);
	IplImage* backImage;
	//USES_CONVERSION;
	//capture = 
	if(capture!=0)
	{
		cvGrabFrame( capture );
		backImage = cvRetrieveFrame( capture );
		size = cvSize(backImage->width,backImage->height);
	}
	
	cvReleaseCapture( &capture );//�ͷ��豸

	int limit = 0;
			
	//��ʼ��
	while(node)
	{
		if(limit++>=LIMIT)
			break;
		USES_CONVERSION;
		node->capture = cvCaptureFromAVI(FilePathName);

		//��ȷ��λ
		cvSetCaptureProperty(node->capture, CV_CAP_PROP_POS_FRAMES, node->startFrame-1);
		cvGrabFrame( node->capture );
		int posFrames = (int) cvGetCaptureProperty(node->capture, CV_CAP_PROP_POS_FRAMES);
		for(int i = 1; posFrames>node->startFrame; i++)
		{
			cvSetCaptureProperty(node->capture, CV_CAP_PROP_POS_FRAMES, node->startFrame-i-1);
			cvGrabFrame( node->capture );
			posFrames = (int) cvGetCaptureProperty(node->capture, CV_CAP_PROP_POS_FRAMES);
		}
		while(posFrames<node->startFrame-1)
		{
			cvGrabFrame( node->capture );
			posFrames = (int) cvGetCaptureProperty(node->capture, CV_CAP_PROP_POS_FRAMES);
		}

        node->eventTempNode = node->eventStart;
		node = node->next;
	}
	node = head;
    
	CvVideoWriter*writer = cvCreateVideoWriter("allEvent.avi",-1,50, size);

	cvNamedWindow( "AllEvent", 1 );//��������
	IplImage* AllEventImage = cvCreateImage(cvGetSize(backImage),backImage->depth , backImage->nChannels);
	cvCopy(backImage,AllEventImage,NULL);
	for(int i = 0; i < maxEvent; i++)
	{
		if(i%JIANGE_FRAME==0)
		{
			IplImage *tempRelease = AllEventImage;
			AllEventImage = cvCreateImage(cvGetSize(backImage),backImage->depth , backImage->nChannels);
			cvCopy(backImage,AllEventImage,NULL);
			cvReleaseImage(&tempRelease);
		}
		limit = 0;
		while(node)
		{
			if(limit++>=LIMIT)
				break;
			if(node->eventTempNode==NULL)
			{
				node = node->next;
				continue;
			}
			if(node->capture)
			{
				IplImage* image;
				if( !(cvGrabFrame(node->capture )))//��׽һ��
					break;
				if(i%JIANGE_FRAME==0 && node->eventTempNode)
				{
					image = cvRetrieveFrame( node->capture );//ȡ�����֡
					if( image )//��ȡ�����ж�motion�Ƿ�Ϊ��
					{
						CvSize pre_size = {node->eventTempNode->rect.width, node->eventTempNode->rect.height};
						CvMat test;
						IplImage*sub_img=cvCreateImage(pre_size,IPL_DEPTH_8U,3);
						cvGetImage(cvGetSubRect(image,&test,node->eventTempNode->rect),sub_img);
						// Set the image ROI to display the current image
						cvSetImageROI(AllEventImage,node->eventTempNode->rect);
						// Resize the input image and copy the it to the Single Big Image
						cvResize(sub_img, AllEventImage);
						// Reset the ROI in order to display the next image
						cvResetImageROI(AllEventImage);
						
					}
					node->eventTempNode = node->eventTempNode->next;
				}
			}
			node = node->next;
		}
		node = head;

		cvWriteToAVI( writer, AllEventImage);

		if(i%JIANGE_FRAME==0)
			cvShowImage( "AllEvent", AllEventImage );//��ʾ�������ͼ��
		if( cvWaitKey(10) >= 0 )//10ms�а�������˳�
			break;
	}
	node = head;

	limit = 0;
	while(node)
	{
		if(limit++>=LIMIT)
			break;
		cvReleaseCapture( &node->capture );//�ͷ��豸
		node = node->next;
	}
	cvDestroyWindow( "AllEvent" );//���ٴ���
	cvReleaseVideoWriter( &writer );
}

/*
//��ʾ�����¼�
static void displayAllEvent2(int total, int maxEvent)
{
	HistNode* node = head;
	CvSize size = cvSize(300,300);
    CvCapture* capture;
	IplImage* backImage;
	USES_CONVERSION;
	capture = cvCaptureFromAVI( FilePathName);
	if(capture)
	{
		cvGrabFrame( capture );
		backImage = cvRetrieveFrame( capture );
		size = cvSize(backImage->width,backImage->height);
	}
	
	cvReleaseCapture( &capture );//�ͷ��豸

	while(node)
	{
		node->capture = cvCaptureFromAVI( FilePathName);
		node->eventTempNode = node->eventStart;

		node = node->next;
	}
	node = head;

	CvVideoWriter*writer = cvCreateVideoWriter("out.avi", -1, 50, size, 1);

	cvNamedWindow("AllEvent", 1);
	IplImage* AllEventImage; //= cvCreateImage(size, IPL_DEPTH_8U, 3);
	//cvCopy(backImage, AllEventImage, NULL);

	for(int i = 0; i < maxEvent; i++)
	{
		if(i%JIANGE_FRAME == 0)
		{
			//IplImage * tempRelease = AllEventImage;
			AllEventImage = cvCreateImage(size, IPL_DEPTH_8U, 3);
			cvCopy(backImage, AllEventImage, NULL);
			//cvReleaseImage(&tempRelease);
		}

		int limit = 0;
		while(node)
		{

			if(limit++ >= LIMIT)
			{
				break;
			}
			if(!node->eventTempNode)
			{
				node = node->next;
				continue;
			}
			if(node->capture)
			{
				IplImage* image;
				if( !(cvGrabFrame(node->capture )))//��׽һ��
					break;
				if(i%JIANGE_FRAME==0)
				{
					image = cvRetrieveFrame( node->capture );//ȡ�����֡
					if( image )//��ȡ�����ж�motion�Ƿ�Ϊ��
					{
						CvMat test;
						cvGetImage(cvGetSubRect(image,&test,node->eventTempNode->rect),image);
						// Set the image ROI to display the current image
						cvSetImageROI(AllEventImage,node->rect);
						// Resize the input image and copy the it to the Single Big Image
						cvResize(image, AllEventImage);
						// Reset the ROI in order to display the next image
						cvResetImageROI(AllEventImage);
						
					}
				}
			}
			node->eventTempNode = node->eventTempNode->next;
			node = node->next;
		}
		node = head;
		cvWriteToAVI( writer, AllEventImage);

		if(i%JIANGE_FRAME==0)
			cvShowImage( "AllEvent", AllEventImage );//��ʾ�������ͼ��
		if( cvWaitKey(10) >= 0 )//10ms�а�������˳�
			break;
	}

	node = head;
	while(node)
	{
		cvReleaseCapture( &node->capture );//�ͷ��豸
		node = node->next;
	}
	cvDestroyWindow( "AllEvent" );//���ٴ���
	cvReleaseVideoWriter( &writer );

}*/
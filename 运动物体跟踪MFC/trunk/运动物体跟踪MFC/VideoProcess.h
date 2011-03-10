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

#include "运动物体跟踪MFC.h"
#include "运动物体跟踪MFCDlg.h"

using namespace std;

class VideoProcess
{
public:
	VideoProcess(void);
	~VideoProcess(void);
};


// various tracking parameters (in seconds) //跟踪的参数（单位为秒）
const double MHI_DURATION = 0.5;//0.5s为运动跟踪的最大持续时间
const double MAX_TIME_DELTA = 0.5;
const double MIN_TIME_DELTA = 0.05;
const int N = 3;
const int PRE_NUM_FRAME = 4;
const int JIANGE_FRAME = 4;
const int LIMIT = 40;
//
const int CONTOUR_MAX_AERA = 60000;
const int CONTOUR_MIN_AERA = 6000;

static struct HistNode*head;
static CString FilePathName;
//  参数：
//  img – 输入视频帧
//  dst – 检测结果
static void  update_mhi( IplImage* img, IplImage* dst, int frameNum, IplImage**&buf, int &last, IplImage*&mhi)
{
	double timestamp = clock()/100.; // get current time in seconds 时间戳
    CvSize size = cvSize(img->width,img->height);
    // get current frame size，得到当前帧的尺寸
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
    // 直接使用CONTOUR中的矩形来画轮廓
    for(;cont;cont = cont->h_next)
    {
		CvRect r = ((CvContour*)cont)->rect;
	  
	    if(r.height * r.width > CONTOUR_MIN_AERA&&r.height * r.width < CONTOUR_MAX_AERA) // 面积小的方形抛弃掉
	    {
			//提取矩形框图像和直方图
		    //CvSize size = {r.width,r.height};
	        //CvMat test;
		    //IplImage*sub_img=cvCreateImage(size,IPL_DEPTH_8U,3);
		    //cvGetImage(cvGetSubRect(img,&test,r),sub_img);
		    //cvShowImage( "dst", sub_img );//建立窗口

			CvScalar s;

			if(frameNum!=-5)
			{
				//链表操作
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
			}else
			{
				s = sampleColor[0];
			}

	        cvRectangle( img, cvPoint(r.x,r.y),
		    cvPoint(r.x + r.width, r.y + r.height), s, 1, CV_AA,0);
	    }
    }
	if(frameNum!=-5)
	{
		bianliNode(head, frameNum);
	}
    // free memory
    cvReleaseMemStorage(&stor);
    cvReleaseImage( &pyr );
}

static void  update_mhi2( IplImage* img, IplImage* dst, int frameNum, IplImage**&buf, int &last, IplImage*&mhi, CvRect &preRect)
{
	double timestamp = clock()/100.; // get current time in seconds 时间戳
    CvSize size = cvSize(img->width,img->height);
    // get current frame size，得到当前帧的尺寸
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
    // 直接使用CONTOUR中的矩形来画轮廓
    for(;cont;cont = cont->h_next)
    {
		CvRect r = ((CvContour*)cont)->rect;
	  
	    if(r.height * r.width > CONTOUR_MIN_AERA&&r.height * r.width < CONTOUR_MAX_AERA) // 面积小的方形抛弃掉
	    {
			//提取矩形框图像和直方图
		    //CvSize size = {r.width,r.height};
	       // CvMat test;
		    //IplImage*sub_img=cvCreateImage(size,IPL_DEPTH_8U,3);
		    //cvGetImage(cvGetSubRect(img,&test,r),sub_img);
		    //cvShowImage( "dst", sub_img );//建立窗口
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

//显示所有事件处理
static void  update_mhi3( IplImage* &img, IplImage* dst, IplImage**&buf, int &last, IplImage*&mhi, CvRect &preRect)
{
	double timestamp = clock()/100.; // get current time in seconds 时间戳
    CvSize size = cvSize(img->width,img->height);
    // get current frame size，得到当前帧的尺寸
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
    // 直接使用CONTOUR中的矩形来画轮廓
    for(;cont;cont = cont->h_next)
    {
		CvRect r = ((CvContour*)cont)->rect;
	  
	    if(r.height * r.width > CONTOUR_MIN_AERA&&r.height * r.width < CONTOUR_MAX_AERA) // 面积小的方形抛弃掉
	    {
			if(isTheSame(r, preRect))
			{
				preRect = r;
			}
	    }
    }
	//提取矩形框图像和直方图
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

	// ring image buffer 圈出图像缓冲
	IplImage **buf = 0;//指针的指针
	int last = 0;
	// temporary images临时图像
	IplImage *mhi = 0; // MHI: motion history image

	USES_CONVERSION;
	capture = cvCaptureFromAVI(str);
    if( capture )
    {
		int totalFrames = (int) cvGetCaptureProperty(capture,  CV_CAP_PROP_FRAME_COUNT);
        cvNamedWindow( "Motion", 1 );//建立窗口
		//cvNamedWindow( "dst", 1 );//建立窗口
        for(int frameNum = 0;;frameNum++)
        {
			p->SetPos(100*frameNum/totalFrames);
            IplImage* image;
			int t = 0;
            if( !(t=cvGrabFrame( capture )))//捕捉一桢
                break;
			if(frameNum%JIANGE_FRAME==0)
			{
				image = cvRetrieveFrame( capture );//取出这个帧
				if( image )//若取到则判断motion是否为空
				{
					//cvFlip(image,image,1);//图像翻转
					if( !motion )
					{
						motion = cvCreateImage( cvSize(image->width,image->height), 8, 1 );
						//创建motion帧，八位，一通道
						cvZero( motion );
						//零填充motion
						motion->origin = image->origin;
						//内存存储的顺序和取出的帧相同
					}
				}
				update_mhi( image, motion, frameNum, buf, last, mhi);//更新历史图像
				cvShowImage( "Motion", image );//显示处理过的图像
				if( cvWaitKey(10) >= 0 )//10ms中按任意键退出
					break;
			}
        }
        cvReleaseCapture( &capture );//释放设备
        cvDestroyWindow( "Motion" );//销毁窗口
		//cvDestroyWindow( "dst" );//销毁窗口
    }
	return;
} 

//显示单个事件视频
static void displaySingleEvent(int index)
{
	HistNode* node = head;
	for(int i = 0; i < index; i++)
	{
		node=node->next;
	}

    IplImage* motion = 0;
    CvCapture* capture = 0;

	// ring image buffer 圈出图像缓冲
	IplImage **buf = 0;//指针的指针
	int last = 0;
	// temporary images临时图像
	IplImage *mhi = 0; // MHI: motion history image

	USES_CONVERSION;
	capture = cvCaptureFromAVI( FilePathName);
	if( capture )
    {
		node->rect = node->startRect;
		//提前两帧
		int startFrame = node->startFrame;
		if((node->startFrame - 4) >= 1)
			startFrame = node->startFrame - 4;
		cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, startFrame);
        cvNamedWindow( "Motion_Event", 1 );//建立窗口
		for(int i = 0;i <= node->endFrame - startFrame;i++)
        {   
			int t;
            IplImage* image;
            if( !(t=cvGrabFrame( capture )))//捕捉一桢
                break;
			if(i%2==0)
			{
				image = cvRetrieveFrame( capture );//取出这个帧
				if( image )//若取到则判断motion是否为空
				{
					//cvFlip(image,image,1);//图像翻转
					if( !motion )
					{
						motion = cvCreateImage( cvSize(image->width,image->height), 8, 1 );
						//创建motion帧，八位，一通道
						cvZero( motion );
						//零填充motion
						motion->origin = image->origin;
						//内存存储的顺序和取出的帧相同
					}
				}
				update_mhi2( image, motion, -5, buf, last, mhi, node->rect);//更新历史图像
				cvShowImage( "Motion_Event", image );//显示处理过的图像
				if( cvWaitKey(50) >= 0 )//10ms中按任意键退出
					break;
			}
		}
		cvReleaseCapture( &capture );//释放设备
		cvDestroyWindow( "Motion_Event" );//销毁窗口
	}
}

//显示所有事件
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
	
	cvReleaseCapture( &capture );//释放设备
	//初始化
	while(node)
	{
		USES_CONVERSION;
		node->capture = cvCaptureFromAVI(FilePathName);

		//提前两帧
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

	cvNamedWindow( "AllEvent", 1 );//建立窗口
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
				if( !(cvGrabFrame(node->capture )))//捕捉一桢
					break;
				if(i%JIANGE_FRAME==0)
				{
					image = cvRetrieveFrame( node->capture );//取出这个帧
					if( image )//若取到则判断motion是否为空
					{
						//cvFlip(image,image,1);//图像翻转
						if( !node->motion )
						{
							node->motion = cvCreateImage( cvSize(image->width,image->height), 8, 1 );
							//创建motion帧，八位，一通道
							cvZero( node->motion );
							//零填充motion
							node->motion->origin = image->origin;
							//内存存储的顺序和取出的帧相同
						}
						update_mhi3( image, node->motion, node->buf, node->last, node->mhi, node->rect);//更新历史图像
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
			cvShowImage( "AllEvent", AllEventImage );//显示处理过的图像
		if( cvWaitKey(10) >= 0 )//10ms中按任意键退出
			break;
	}
	node = head;
	while(node)
	{
		cvReleaseCapture( &node->capture );//释放设备
		node = node->next;
	}
	cvDestroyWindow( "AllEvent" );//销毁窗口
	cvReleaseVideoWriter( &writer );
}

//显示所有事件
static void displayAllEvent2(int total, int maxEvent)
{
	HistNode* node = head;
	CvSize size = cvSize(300,300);
	double fps = 1;
    CvCapture* capture = 0;
	USES_CONVERSION;
	capture = cvCaptureFromAVI( FilePathName);
	if(capture)
	{
		size = cvSize((int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH),(int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH));
		fps = cvGetCaptureProperty (capture,CV_CAP_PROP_FPS);
	}
	
	cvReleaseCapture( &capture );//释放设备
	//初始化
	while(node)
	{
		USES_CONVERSION;
		node->capture = cvCaptureFromAVI(FilePathName);

		//提前两帧
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

	//建立最初的事件合集视频（用最长事件作为时间）
	//CvVideoWriter * writer = cvCreateVideoWriter("allevent1.avi", /*CV_FOURCC('X','V','I','D'),*/-1, fps, size, 1);
	CvVideoWriter * writer = cvCreateVideoWriter("allevent1.avi", CV_FOURCC('X','V','I','D'), fps, size, 1);
	IplImage* AllEventImage = cvCreateImage( size, IPL_DEPTH_8U, 3 );
	for(int i = 0; i < maxEvent + PRE_NUM_FRAME; i++)
	{
		cvWriteFrame(writer, AllEventImage);
	}
	cvReleaseVideoWriter(&writer);
	while(node)
	{
		//每执行一次循环就把一个事件放入视频中
		CvCapture*capture2 = cvCaptureFromAVI( "allevent.avi");
		CvVideoWriter*writer2 = cvCreateVideoWriter("alleventtemp.avi", CV_FOURCC('X','V','I','D'), fps, size, 1);
		//提前4帧
		int startFrame = node->startFrame;
		if((node->startFrame - PRE_NUM_FRAME) >= 1)
			startFrame = node->startFrame - PRE_NUM_FRAME;
		cvSetCaptureProperty(node->capture, CV_CAP_PROP_POS_FRAMES, startFrame);
		
		IplImage *AllEventImage;
		for(int i = 0; i < maxEvent + PRE_NUM_FRAME; i++)
		{
			AllEventImage = cvQueryFrame(capture2);
			if(i<node->endFrame - node->startFrame+PRE_NUM_FRAME)
			{
				IplImage* image;
				if( !(cvGrabFrame(node->capture )))//捕捉一桢
					break;
				if(i%2==0)
				{
					image = cvRetrieveFrame( node->capture );//取出这个帧
					if( image )//若取到则判断motion是否为空
					{
						//cvFlip(image,image,1);//图像翻转
						if( !node->motion )
						{
							node->motion = cvCreateImage( cvSize(image->width,image->height), 8, 1 );
							//创建motion帧，八位，一通道
							cvZero( node->motion );
							//零填充motion
							node->motion->origin = image->origin;
							//内存存储的顺序和取出的帧相同
						}
						update_mhi3( image, node->motion, node->buf, node->last, node->mhi, node->rect);//更新历史图像
						// Set the image ROI to display the current image
						cvSetImageROI(AllEventImage,node->rect);
						// Resize the input image and copy the it to the Single Big Image
						cvResize(image, AllEventImage);
						// Reset the ROI in order to display the next image
						cvResetImageROI(AllEventImage);
						
					}
				}
			}

			cvWriteFrame(writer2, AllEventImage);
		}

		cvReleaseVideoWriter(&writer2);
		cvReleaseCapture(&capture2);

		DeleteFile("allevent.avi");
		rename("alleventtemp.avi", "allevent.avi");
		node = node->next;
	}
	node = head;
}
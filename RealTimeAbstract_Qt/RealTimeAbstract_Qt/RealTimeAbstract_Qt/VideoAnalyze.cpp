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

//视频内容检索
void VideoAnalyze::videoSearch()
{
	//第一步：删除原界面中的摘要事件
	emit sendRemoveAllAbstracts();
	emit sendAnalyzeButtonState(1);
	//第二步：解析data数据
	QStringList dataList = data.split("|");
	//1解析时间数据
	int isTimeLimit = 0;
	{
		QStringList subDataList = dataList[0].split(",");
		if(subDataList[0].compare("1")==0)
		{
			isTimeLimit = 1;
		}
	}
	//2解析运动特征
	int isMotionLimit = 0;
	{
		if(dataList[1].compare("1")==0)
		{
			isMotionLimit = 1;
		}
	}
	//3解析颜色特征
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
	//4解析类别特征
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
	
	//初始化
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

	if(isTimeLimit)//时间筛选
	{
	}
	if(isMotionLimit)//运动特征筛选
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
	if(isColorLimit)//颜色筛选
	{
		colorFilter(r,g,b);
	}
	if(isTypeLimit)//类别筛选
	{
		typeFilter(type);
	}
	
	//第四步：重画摘要
	drawAbstracts();
	release();
	emit sendAnalyzeButtonState(0);
}

//类别过滤
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
		if(type == 0)  //人
		{
			mark = isHumanEvent((*iter), human_detector);
		}else if(type == 1) //车
		{
			mark = isCarEvent((*iter), car_detector);
		}else if(type == 2) //其它
		{
			mark = !isCarEvent((*iter), car_detector) && !isHumanEvent((*iter), human_detector);
		}
		if(!mark)  //删除不是检索的对象
			iter = eventList.erase(iter);
		else
			iter++ ;
	}
}

//颜色过滤器
void VideoAnalyze::colorFilter(int r, int g, int b)
{
    IplImage* pFrImg1 = NULL;
    IplImage* pFrImg2 = NULL;
    IplImage* pFrImg3 = NULL;
    IplImage* result1 = NULL;
    IplImage* result2 = NULL;
    IplImage* result = NULL;
	IplImage* pre_frame = NULL;
	CvMat* pFrMat1 = NULL;
    CvMat* pFrMat2 = NULL;
    CvMat* pFrMat3 = NULL;
    CvMat* reMat1 = NULL;
    CvMat* reMat2 = NULL;
    CvMat* reMat = NULL;

	pFrImg1 = cvCreateImage(captureSize, IPL_DEPTH_8U, 1);
    pFrImg2 = cvCreateImage(captureSize, IPL_DEPTH_8U, 1);
    pFrImg3 = cvCreateImage(captureSize, IPL_DEPTH_8U, 1);
    result1 = cvCreateImage(captureSize, IPL_DEPTH_8U, 1);
    result2 = cvCreateImage(captureSize, IPL_DEPTH_8U, 1);
    result = cvCreateImage(captureSize, IPL_DEPTH_8U, 1);
	pre_frame = cvCreateImage(captureSize, IPL_DEPTH_8U, 3);

	pFrMat1 = cvCreateMat(captureSize.height, captureSize.width, CV_8UC1);
    pFrMat2 = cvCreateMat(captureSize.height, captureSize.width, CV_8UC1);
    pFrMat3 = cvCreateMat(captureSize.height, captureSize.width, CV_8UC1);
    reMat1 = cvCreateMat(captureSize.height, captureSize.width, CV_8UC1);
    reMat2 = cvCreateMat(captureSize.height, captureSize.width, CV_8UC1);
    reMat = cvCreateMat(captureSize.height, captureSize.width, CV_8UC1);

	for(vector<EventNode>::iterator iter=eventList.begin(); iter!=eventList.end(); )
	{
		EventNode node = *iter;
		int frameNum = node.startFrame+node.trackList.size()/3*jiange-2;//从运动事件的三分之一开始
		cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, frameNum);
		//先读取两帧
		frame = cvQueryFrame( capture );
		frameNum++;
		cvCvtColor(frame, pFrImg1, CV_BGR2GRAY);
		cvConvert(pFrImg1, pFrMat1);
		for(int i = jiange; i > 1; i--)
		{
			frame=cvQueryFrame( capture );
			frameNum++;
		}
		frame = cvQueryFrame( capture );
		frameNum++;
		cvCvtColor(frame, pFrImg2, CV_BGR2GRAY);
		cvConvert(pFrImg2, pFrMat2);

		cvCopy(frame, pre_frame);

		int count = 0;
		for(int i = frameNum; i < node.endFrame; i++)//检查帧
		{
			frame = cvQueryFrame(capture);
			if(i%jiange==0)
			{
				cvCvtColor(frame, pFrImg3, CV_BGR2GRAY);
				cvConvert(pFrImg3, pFrMat3);
				//当前帧跟前一帧相减
				cvAbsDiff(pFrMat2, pFrMat1, reMat1);
				cvAbsDiff(pFrMat3, pFrMat2, reMat2);
				//二值化前景图
			    cvThreshold(reMat1, result1, 10.0, 255.0, CV_THRESH_BINARY);
				cvThreshold(reMat2, result2, 10.0, 255.0, CV_THRESH_BINARY);
				//两个帧差值相与
				cvAnd(result1,result2,result,0);

				int temp_index = (frameNum-node.startFrame)/jiange;
				int sx_point = node.trackList[temp_index].x;
				int sy_point = node.trackList[temp_index].y;
				int ex_point = node.trackList[temp_index].width;
				int ey_point = node.trackList[temp_index].height;
				int same_color_count = 0;
				int color_count = 0;
				for(int y = sy_point; y < sy_point+ey_point; y++)
				{
					for(int x = sx_point; x < sx_point+ex_point; x++)
					{
						int step    = result->widthStep/sizeof(uchar);
						uchar* data = (uchar *)result->imageData;
						if(data[y*step+x] >= 200)
						{
							color_count++;
							int step       = pre_frame->widthStep/sizeof(uchar);
							int channels   = pre_frame->nChannels;
							uchar* data    = (uchar *)pre_frame->imageData;
							int G = data[y*step+x*channels+0];
							int B = data[y*step+x*channels+1];
							int R = data[y*step+x*channels+2];
							if(isTheSameRGBColor(r,g,b,R,G,B))
								same_color_count++;
						}
					}
				}
				float temp_result = 1.0*same_color_count / color_count;
				if(temp_result >= 0.2)
					count++;
			}
			frameNum++;
			cvCopy(frame, pre_frame);
			cvCopy(pFrMat2, pFrMat1, NULL);
			cvCopy(pFrMat3, pFrMat2, NULL);
			cvWaitKey(1);
		}

		if(count < 5)  //删除不是检索的对象
			iter = eventList.erase(iter);
		else
			iter++;
	}
	//释放图像和矩阵
    cvReleaseImage(&pFrImg1);
    cvReleaseImage(&pFrImg2);
	cvReleaseImage(&pFrImg3);
    cvReleaseImage(&result1);
    cvReleaseImage(&result2);
    cvReleaseImage(&result);
    cvReleaseImage(&pre_frame);

    cvReleaseMat(&pFrMat1);
    cvReleaseMat(&pFrMat2);
	cvReleaseMat(&pFrMat3);
    cvReleaseMat(&reMat1);
    cvReleaseMat(&reMat2);
    cvReleaseMat(&reMat);
}

//判断是不是同一个rgb颜色
bool VideoAnalyze::isTheSameRGBColor(int r1, int g1, int b1, int r2, int g2, int b2) 
{
	int w_r = 1, w_g = 2, w_b = 1;
	int r1addr2 = r1 + r2;
	int g1addg2 = g1 + g2;
	int b1addb2 = b1 + b2;
	int r1subr2 = abs(r1 - r2);
	int g1subg2 = abs(g1 - g2);
	int b1subb2 = abs(b1 - b2);
	float sum_rgb = (r1addr2 + g1addg2 + b1addb2)/3.0;
	float s_r = r1addr2/sum_rgb;
	s_r = s_r < 1 ? s_r : 1;
	float s_r_2 = s_r*s_r;
	float s_g = g1addg2/sum_rgb;
	s_g = s_g < 1 ? s_g : 1;
	float s_g_2 = s_g*s_g;
	float s_b = b1addb2/sum_rgb;
	s_b = s_b < 1 ? s_b : 1;
	float s_b_2 = s_b*s_b;
	float theta = 2*acos((r1*r2 + g1*g2 + b1*b2)/(sqrt(1.0*(r1*r1+g1*g1+b1*b1)*(r2*r2+g2*g2+b2*b2))))/3.14159;
	float tempadd = 1.0*r1subr2/r1addr2 + 1.0*g1subg2/g1addg2 + 1.0*b1subb2/b1addb2;
	float s_theta_r = r1subr2/r1addr2/tempadd*s_r*s_r;
	float s_theta_g = g1subg2/g1addg2/tempadd*s_g*s_g;
	float s_theta_b = b1subb2/b1addb2/tempadd*s_b*s_b;
	float s_theta = s_theta_r + s_theta_g + s_theta_b;
	//选出最大值
	int max = r1;
	max = max > r2 ? max : r2;
	max = max > g1 ? max : g1;
	max = max > g2 ? max : g2;
	max = max > b1 ? max : b1;
	max = max > b2 ? max : b2;

	float s_max_theta = max*1.0/255;

	float dist = sqrt((s_r_2*w_r*r1subr2*r1subr2 + s_g_2*w_g*g1subg2*g1subg2 + s_b_2*w_b*b1subb2*b1subb2)/((w_r+w_g+w_b)*255*255)+s_theta*s_max_theta*theta*theta);

	if(dist <= 0.25)
		return true;
	else
		return false;
}

//判断事件是不是人物事件
bool VideoAnalyze::isHumanEvent(EventNode node, CascadeClassifier human_detector)
{
	int frameNum = node.startFrame+node.trackList.size()/3*jiange;
	//int frameNum = (*iter).startFrame;
	cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, frameNum);
	int count = 0;
	for(int i = frameNum; i < node.endFrame; i++)//检查帧
	{
		frame = cvQueryFrame(capture);
		if(i%jiange==0)
		{
			std::vector<Rect> detectRects;
			Mat fImage(frame,0);
			//car_detector.detectMultiScale(fImage, detectRects, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, Size(100, 100), Size(1000,1000) );
			human_detector.detectMultiScale(fImage, detectRects, 1.1, 1, 0|CV_HAAR_SCALE_IMAGE, Size(9, 30), Size(45,150) );
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
		}
		frameNum++;
	}
	return false;
}

//判断是不是车辆事件
bool VideoAnalyze::isCarEvent(EventNode node, CascadeClassifier car_detector)
{
	//int frameNum = node.startFrame+node.trackList.size()/3*jiange;
	int frameNum = node.startFrame;
	cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, frameNum);
	int count = 0;
	for(int i = frameNum; i < node.endFrame; i++)//检查帧
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
		}
		frameNum++;
	}
	return false;
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
		info = tr("正在分析视频:") + fileName;
		emit sendProcessInfo(info);
		this->analyzeVideo();
		this->saveEventToFile();
		info = tr("正在生成全摘要列表");
		emit sendProcessInfo(info);
		this->drawAbstracts();
		info = tr("正在生成全摘要视频");
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
		//this->createAllEventVideo();//测试代码
	}
	release();
}

void VideoAnalyze::batchAnalysis()
{
	for(int i = 0; i < filePathList.size(); i++)
	{
		filePath = filePathList[i];
		//更新正在分析的视频信息
		QString info = tr("正在分析第") + QString::number(i+1, 10) + tr("个文件，共") + QString::number(Globals::files.size(), 10) + tr("个文件。");
		emit sendProcessInfo(info);

		//判断分析文件是否存在
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
	//初始化摄像头视频
	initRealTime();
	analyzeRealTimeVideo();
	filePath = tr("videowrite.avi"); //必须在写入文本文件之前保存
	//emit sendProcessInfo(tr("开始写入文件"));
	this->saveEventToFile();
	release();
	init();
	//emit sendProcessInfo(tr("开始生成所有摘要视频"));
	this->createAllEventVideo();
	release();
}

void VideoAnalyze::analyzeRealTimeVideo()
{
	int N = 3;
	IplImage*motion = 0;   //内存未释放！
	IplImage**buf = 0;
	IplImage*mhi = 0;
	int last = 0;
	double lastTime = 0;
	if(capture)
	{
		//定义一个写视频对象
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
				msleep(100);//这里如果不暂停的话，会因为现实图片被释放而出现内存错误
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
	IplImage*motion = 0;   //内存未释放！
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
		while (true)
		{
			frame = cvQueryFrame(capture);
			if (!frame || !isContinue)
			{
				emit sendProcessBarValue(100);
				msleep(100);//这里如果不暂停的话，会因为现实图片被释放而出现内存错误
				break;
			}
			//获取第五帧为baseFrame
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
	QString videoPath, videoName;
	Globals::getFileDirFromQString(filePath, videoPath);
	Globals::getFileNameFromQString(filePath, videoName);
	videoPath = videoPath + tr("analyze\\") + videoName;
	QByteArray ba = videoPath.toLocal8Bit();
	char* path = ba.data();
	videoWriter = cvCreateVideoWriter(path, CV_FOURCC('X', 'V', 'I', 'D'), fps, captureSize, 1);
	if(!videoWriter)
	{
		emit sendProcessInfo(tr("无法创建视频摘要，可能没有编码器支持!"));
		return;
	}
	int part = eventList.size() / LIMIT;
	if(eventList.size()%LIMIT!=0)
		part++;
	//将第一帧作为背景图像
	double alpha_value = 0.7;
	char eventNumber[128];
	CvFont font;
    cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0.5, 1, CV_AA);
	IplImage* allEventImage = cvCreateImage(captureSize, 8, 3);

	//将视频分成part端，分别对没段进行合成保存
	for(int i = 0; i < part; i++)
	{
		//计算每一段的下标
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
		//对每一段视频进行合成保存    记得要处理帧间隔
		int frameCount = 0;   //记录帧偏移量
		int endCount = 0;     //记录有几个节点已经完成

		while(endCount < r_index-l_index)
		{
			cvCopy(baseFrame, allEventImage);

			for(int j = l_index; j < r_index; j++)
			{
				int frameNum = eventList[j].startFrame+frameCount*jiange;
				if(frameCount == eventList[j].trackList.size())  //注意这里一定是“==”，如果是“>=”的话会使endCount重复多加几次，损失很多帧
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
				//截取矩形图像合成
				cvSetImageROI(frame, rect);
				sprintf(eventNumber, "%d", j+1);
				cvPutText(frame, eventNumber, cvPoint(10, 15), &font, EventNodeOperation::sampleColor[1]);
				cvSetImageROI(allEventImage, rect);
				cvAddWeighted(frame, alpha_value, allEventImage, 1 - alpha_value, 0, allEventImage);
				cvResetImageROI(allEventImage);  //去掉这一句会怎样？
				cvResetImageROI(frame);
			}
			for(int j = 0; j < jiange; j++)  //重复写入jiange帧
			{
				cvWriteFrame(videoWriter, allEventImage);
			}
			frameCount++;
		}
	}
	//记得释放空间
	cvReleaseVideoWriter(&videoWriter);
	cvReleaseImage(&allEventImage);
	emit sendProcessInfo(tr("分析结束！"));
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
	//CvInvoke.cvQueryFrame(capture);//2.4版本必须加这句才能进行cvSetCaptureProperty操作

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
	int framePosition;
	if(isSaveToFile)
	{
		for(int i = 0; i < eventList.size(); i++)
		{
			EventNode node = eventList[i];
			Rect rect = node.trackList[node.trackList.size()/2];
			cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, node.startFrame+node.trackList.size()/2*jiange);
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
			QString startTime = tr("开始时间:") + Globals::getTimeFromFrameNum(node.startFrame, fps);
			QString endTime = tr("结束时间:") + Globals::getTimeFromFrameNum(node.endFrame, fps);
			emit sendDrawAbstracts(*qImg, startTime, endTime, i);
			msleep(100);
		}
	}
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
	fps = 15;   //设置实时拍摄帧率为15
	qImg = new QImage(QSize(captureSize.width,captureSize.height), QImage::Format_RGB888);
	iplImg = cvCreateImageHeader(captureSize,  8, 3);
	iplImg->imageData = (char*)qImg->bits();
	//获取基础帧
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
	if(iplImg)   //这里可能会出错，因为QImage和IplImage是绑定的，所以设置断点测试
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
	//开始筛选操作
	flag = VideoAnalyze::SELECTEVENT;
	start();
}

VideoAnalyze::VideoAnalyze(QObject* parent = 0):QThread(parent)
{
	fps = 0;
	minArea = 1000;
	maxArea = 100000;
	jiange = 1;
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
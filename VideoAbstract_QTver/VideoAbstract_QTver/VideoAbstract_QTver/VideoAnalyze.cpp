#include "VideoAnalyze.h"

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
				//update_mhi(ref frame, ref motion, frameNum, ref buf, ref last, ref mhi, captureSize, ref lastTime);
				//CvInvoke.cvShowImage("analyze", frame);
				//CvInvoke.cvWaitKey(100);
			}
			frameNum++;
		}
		cvDestroyAllWindows();
	}
	else
	{
		//QMessageBox::warning(this, tr("错误"), tr("视频文件损坏或格式不正确，无法打开！"));
	}

	//keyFrameJiange(filePath);
}

VideoAnalyze::VideoAnalyze(QString filePath)
{
	this->filePath = filePath;
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

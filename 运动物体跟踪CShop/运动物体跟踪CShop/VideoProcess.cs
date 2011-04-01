using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using Emgu.CV;
using Emgu.CV.Structure;
using Emgu.CV.UI;
namespace 运动物体跟踪CShop
{
    class VideoProcess
    {
        //  参数：
        //  img – 输入视频帧
        //  dst – 检测结果
        static void update_mhi(ref IntPtr img, ref IntPtr dst, int frameNum, ref IntPtr [] buf, ref int last, ref IntPtr mhi, Size size, ref double lastTime)
        {
            int N = 3;
            //double MHI_DURATION = 0.5;
            DateTime.Now.ToShortTimeString();
            DateTime dt = DateTime.Now;
            double timestamp = 0;
            double nowTime = (double)dt.Ticks / 1000;
            if (lastTime == 0)
            {
                timestamp = 0;
                lastTime = nowTime;
            }
            else
            {
                timestamp = nowTime - lastTime;
                lastTime = nowTime;
            }
            int idx1, idx2;
            IntPtr silh;
            IntPtr pyr = CvInvoke.cvCreateImage(new Size((size.Width & -2) / 2, (size.Height & -2) / 2), Emgu.CV.CvEnum.IPL_DEPTH.IPL_DEPTH_8U, 1);
            IntPtr stor;// = CvInvoke.cvCreateMemStorage(0);
            IntPtr cont;

            CvInvoke.cvCvtColor(img, buf[last], Emgu.CV.CvEnum.COLOR_CONVERSION.CV_BGR2GRAY);

            idx1 = last;
            idx2 = (last + 1) % N;
            last = idx2;

            //作帧差
            silh = buf[idx2];
            CvInvoke.cvAbsDiff(buf[idx1], buf[idx2], silh);

            //对差图像做二值化
            CvInvoke.cvThreshold(silh, silh, 30, 255, Emgu.CV.CvEnum.THRESH.CV_THRESH_BINARY);

            //CvInvoke.cvUpdateMotionHistory(silh, mhi, timestamp, MHI_DURATION);

            
            CvInvoke.cvConvert(silh, dst);
            //CvInvoke.cvConvert(mhi, dst);


            //中值滤波
            CvInvoke.cvSmooth(dst, dst, Emgu.CV.CvEnum.SMOOTH_TYPE.CV_MEDIAN, 3, 0, 0, 0);

            CvInvoke.cvPyrDown(dst, pyr, Emgu.CV.CvEnum.FILTER_TYPE.CV_GAUSSIAN_5x5);
            CvInvoke.cvDilate(pyr, pyr, new IntPtr(0), 1);
            CvInvoke.cvPyrUp(pyr, dst, Emgu.CV.CvEnum.FILTER_TYPE.CV_GAUSSIAN_5x5);

            //下面程序用来找轮廓
            stor = CvInvoke.cvCreateMemStorage(0);
            cont = IntPtr.Zero;

            int n = CvInvoke.cvFindContours(dst, stor, ref cont, System.Runtime.InteropServices.Marshal.SizeOf(typeof(MCvContour)), Emgu.CV.CvEnum.RETR_TYPE.CV_RETR_LIST, 
                Emgu.CV.CvEnum.CHAIN_APPROX_METHOD.CV_CHAIN_APPROX_SIMPLE, new Point(0, 0));
            Seq<Point> DyncontourTemp = new Seq<Point>(cont, null);
            for (; DyncontourTemp != null && DyncontourTemp.Ptr.ToInt32() != 0; DyncontourTemp = DyncontourTemp.HNext)
            {
                Rectangle r = DyncontourTemp.BoundingRectangle;
                if (r.Height * r.Width > Global.minArea && r.Height * r.Width < Global.maxArea)
                {
                    MCvScalar s;

                    EventNode node = new EventNode(); ;
                    if (Global.eventList.Count == 0)
                    {
                        EventNodeOperation.insertEventNode(ref Global.eventList, r, frameNum);
                    }
                    else
                    {
                        node = EventNodeOperation.searchEventList(ref Global.eventList, r);
                        if(node == null)
                            node = EventNodeOperation.insertEventNode(ref Global.eventList, r, frameNum);
                    }
                    s = EventNodeOperation.sampleColor[node.startFrame % 5];

                    CvInvoke.cvRectangle(img, new Point(r.X, r.Y), new Point(r.X + r.Width, r.Y + r.Height), s, 1, Emgu.CV.CvEnum.LINE_TYPE.CV_AA, 0);
                }
            }

            EventNodeOperation.bianliEventList(ref Global.eventList, frameNum);

            CvInvoke.cvReleaseMemStorage(ref stor);
            CvInvoke.cvReleaseImage(ref pyr);
            
        }

        //分析视频
        static public void analyzeVideo(string filePath, VideoMainForm form)
        {
            int N = 3;
            IntPtr capture = CvInvoke.cvCreateFileCapture(filePath);
            IntPtr motion = new IntPtr();
            IntPtr []buf;
            IntPtr mhi;
            int last = 0;
            double lastTime = 0;
           
            if (capture.ToInt32() != 0)
            {
                Size captureSize = new Size((int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_WIDTH),
                    (int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_HEIGHT));

                mhi = CvInvoke.cvCreateImage(captureSize, Emgu.CV.CvEnum.IPL_DEPTH.IPL_DEPTH_32F, 1);
                CvInvoke.cvZero(mhi);
                buf = new IntPtr[N];
                for (int i = 0; i < N; i++)
                {
                    CvInvoke.cvReleaseImage(ref buf[i]);
                    buf[i] = CvInvoke.cvCreateImage(captureSize, Emgu.CV.CvEnum.IPL_DEPTH.IPL_DEPTH_8U, 1);
                    CvInvoke.cvZero(buf[i]);
                }

                CvInvoke.cvNamedWindow("analyze");
                int totalFrames = (int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_COUNT);
                IntPtr frame = new IntPtr();
                int frameNum = 0;
                while (true)
                {
                    //VideoMainForm.analyzeProgressBar.Increment((int)100*frameNum/totalFrames);
                    form.analyzeProgressBar.Value = (int)100 * frameNum / totalFrames;
                    frame = CvInvoke.cvQueryFrame(capture);
                    if (frame.ToInt32() == 0)
                    {
                        form.analyzeProgressBar.Value = 100;
                        break;
                    }
                    if (frameNum % Global.jiange == 0)
                    {
                        if (frame.ToInt32() != 0)
                        {
                            if (motion.ToInt32() == 0)
                            {
                                motion = CvInvoke.cvCreateImage(captureSize, Emgu.CV.CvEnum.IPL_DEPTH.IPL_DEPTH_8U, 1);
                                CvInvoke.cvZero(motion);
                            }
                        }
                        update_mhi(ref frame, ref motion, frameNum, ref buf, ref last, ref mhi, captureSize, ref lastTime);
                        CvInvoke.cvShowImage("analyze", frame);
                        CvInvoke.cvWaitKey(10);
                    }
                    frameNum++;
                }
                CvInvoke.cvDestroyWindow("analyze");
            }

            CvInvoke.cvReleaseCapture(ref capture);
            
        }

        //播放单个事件
        static public void playSingleEvent(int index)
        {
            EventNode eventNode = Global.eventList[index];
            IntPtr capture = CvInvoke.cvCreateFileCapture(Global.filePath);
            if (capture.ToInt32() != 0)
            {
                //精确定位
                CvInvoke.cvSetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_POS_FRAMES, eventNode.startFrame);
                int posFrames = 0;
                CvInvoke.cvQueryFrame(capture);
                posFrames = (int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_POS_FRAMES);
                for (int i = 6; posFrames > eventNode.startFrame; i+=6)
                {
                    CvInvoke.cvSetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_POS_FRAMES, eventNode.startFrame-i);
                    CvInvoke.cvQueryFrame(capture);
                    posFrames = (int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_POS_FRAMES);
                }
                while (posFrames < eventNode.startFrame)
                {
                    CvInvoke.cvQueryFrame(capture);
                    posFrames = (int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_POS_FRAMES);
                }

                string eventName = "事件" + index.ToString();
                CvInvoke.cvNamedWindow(eventName);
                int total = 0;
                while (posFrames <= eventNode.endFrame)
                {
                    IntPtr image = CvInvoke.cvQueryFrame(capture);
                    if (posFrames % Global.jiange == 0 && total < eventNode.trackList.Count)
                    {
                        MCvScalar s = new MCvScalar(255, 0, 0);
                        CvInvoke.cvRectangle(image, new Point(eventNode.trackList[total].X, eventNode.trackList[total].Y),
                            new Point(eventNode.trackList[total].X + eventNode.trackList[total].Width, eventNode.trackList[total].Y + eventNode.trackList[total].Height), s, 1,
                            Emgu.CV.CvEnum.LINE_TYPE.CV_AA, 0);
                        CvInvoke.cvShowImage(eventName, image);
                        CvInvoke.cvWaitKey(20);
                        total++;
                    }
                    posFrames = (int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_POS_FRAMES);
                    //CvInvoke.cvReleaseImage(ref image);
                }
                CvInvoke.cvDestroyWindow(eventName);
            }
            CvInvoke.cvReleaseCapture(ref capture);
        }

        //播放所有事件
        static public void playAllEvents()
        {
            IntPtr capture = CvInvoke.cvCreateFileCapture(Global.filePath);
            if (capture.ToInt32() == 0)
            {
                MessageBox.Show("无法打开视频文件");
                return;
            }
            Size captureSize = new Size((int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_WIDTH),
                    (int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_HEIGHT));
            CvInvoke.cvSetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_POS_FRAMES, 15);
            IntPtr backGroundImage = CvInvoke.cvQueryFrame(capture);

            int limit = 0;

            //初始化
            for (int i = 0; i < Global.eventList.Count; i++)
            {
                if (limit++ >= Global.LIMIT)
                    break;
                Global.eventList[i].capture = CvInvoke.cvCreateFileCapture(Global.filePath);

                //精确定位
                CvInvoke.cvSetCaptureProperty(Global.eventList[i].capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_POS_FRAMES, Global.eventList[i].startFrame);
                int posFrames = 0;
                CvInvoke.cvQueryFrame(Global.eventList[i].capture);
                posFrames = (int)CvInvoke.cvGetCaptureProperty(Global.eventList[i].capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_POS_FRAMES);
                for (int j = 6; posFrames > Global.eventList[i].startFrame; j += 6)
                {
                    CvInvoke.cvSetCaptureProperty(Global.eventList[i].capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_POS_FRAMES, Global.eventList[i].startFrame - j);
                    CvInvoke.cvQueryFrame(Global.eventList[i].capture);
                    posFrames = (int)CvInvoke.cvGetCaptureProperty(Global.eventList[i].capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_POS_FRAMES);
                }
                while (posFrames < Global.eventList[i].startFrame)
                {
                    CvInvoke.cvQueryFrame(Global.eventList[i].capture);
                    posFrames = (int)CvInvoke.cvGetCaptureProperty(Global.eventList[i].capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_POS_FRAMES);
                }
            }

            IntPtr writer = CvInvoke.cvCreateVideoWriter("allEvents.avi", CvInvoke.CV_FOURCC('X', 'V', 'I', 'D'), 20, captureSize, 1);
            CvInvoke.cvNamedWindow("AllEvents");

            IntPtr allEventImage = CvInvoke.cvCreateImage(captureSize, Emgu.CV.CvEnum.IPL_DEPTH.IPL_DEPTH_8U, 3);
            CvInvoke.cvCopy(backGroundImage, allEventImage, new IntPtr());

            double alpha_value = 0.7;
            string event_str = "";
            MCvFont font = new MCvFont();
            CvInvoke.cvInitFont(ref font, Emgu.CV.CvEnum.FONT.CV_FONT_HERSHEY_SIMPLEX, 0.5, 0.5, 0.5, 1, Emgu.CV.CvEnum.LINE_TYPE.CV_AA);
            for (int i = 0; i < Global.maxEventNum; i++)
            {
                if (i % Global.jiange == 0)
                {
                    IntPtr tempRelease = allEventImage;
                    allEventImage = CvInvoke.cvCreateImage(captureSize, Emgu.CV.CvEnum.IPL_DEPTH.IPL_DEPTH_8U, 3);
                    CvInvoke.cvCopy(backGroundImage, allEventImage, new IntPtr());
                    CvInvoke.cvReleaseImage(ref tempRelease);
                }

                limit = 0;
                for (int j = 0; j < Global.eventList.Count; j++)
                {
                    if (limit >= Global.LIMIT)
                        break;
                    if (i/Global.jiange >= Global.eventList[j].trackList.Count)
                        continue;
                    if (Global.eventList[j].capture.ToInt32() != 0 )
                    {
                        IntPtr image = CvInvoke.cvQueryFrame(Global.eventList[j].capture);
                        if (image.ToInt32() != 0 && i % Global.jiange == 0)
                        {
                            Size pre_size = new Size(Global.eventList[j].trackList[i / Global.jiange].Width, Global.eventList[j].trackList[i / Global.jiange].Height);
                            IntPtr sub_img = CvInvoke.cvCreateImage(pre_size, Emgu.CV.CvEnum.IPL_DEPTH.IPL_DEPTH_8U, 3);
                            //CvInvoke.cvGetImage(CvInvoke.cvGetSubRect(image, test.refcount, Global.eventList[j].trackList[i]), sub_img);
                            CvInvoke.cvGetSubRect(image, sub_img, Global.eventList[j].trackList[i / Global.jiange]);
                            event_str = j.ToString();
                            CvInvoke.cvPutText(sub_img, event_str, new Point(10, 15), ref font, EventNodeOperation.sampleColor[1]);

                            CvInvoke.cvSetImageROI(allEventImage, Global.eventList[j].trackList[i / Global.jiange]);
                            CvInvoke.cvAddWeighted(sub_img, alpha_value, allEventImage, 1 - alpha_value, 0, allEventImage);
                            CvInvoke.cvResetImageROI(allEventImage);
                        }
                    }
                }

                CvInvoke.cvWriteFrame(writer, allEventImage);

                if (i % Global.jiange == 0)
                {
                    CvInvoke.cvShowImage("AllEvents", allEventImage);
                    CvInvoke.cvWaitKey(20);
                }
                
            }

            //释放空间
            limit = 0;
            for (int i = 0; i < Global.eventList.Count; i++)
            {
                if (limit++ >= Global.LIMIT)
                    break;
                CvInvoke.cvReleaseCapture(ref Global.eventList[i].capture);
            }
            CvInvoke.cvDestroyWindow("AllEvents");
            CvInvoke.cvReleaseCapture(ref capture);
            CvInvoke.cvReleaseVideoWriter(ref writer);
        }

        //摄像头保存视频
        static public void cameralSaveVideo()
        {
            IntPtr capture = CvInvoke.cvCreateCameraCapture(0);
            IntPtr frame = new IntPtr();
            if (capture.ToInt32() == 0)
            {
                MessageBox.Show("没有发现摄像头");
                return;
            }
            //double fps = CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FPS);
            int fps = 25;
            Size captureSize = new Size((int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_WIDTH),
                    (int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_HEIGHT));

            IntPtr write = CvInvoke.cvCreateVideoWriter("cameral.avi", CvInvoke.CV_FOURCC('X', 'V', 'I', 'D'), fps, captureSize, 1);

            CvInvoke.cvNamedWindow("摄像头");

            for (int i = 0; i < 10000; i++)
            {
                frame = CvInvoke.cvQueryFrame(capture);
                CvInvoke.cvWriteFrame(write, frame);
                CvInvoke.cvShowImage("摄像头", frame);
                if(CvInvoke.cvWaitKey(25) > 0)
                    break ;
            }

            CvInvoke.cvDestroyWindow("摄像头");
            CvInvoke.cvReleaseImage(ref frame);
            CvInvoke.cvReleaseCapture(ref capture);
            CvInvoke.cvReleaseVideoWriter(ref write);

        }
    }
}


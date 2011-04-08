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

    class CarCountProcess
    {
        //  参数：
        //  img – 输入视频帧
        //  dst – 检测结果
        static void update_mhi(ref IntPtr img, ref IntPtr dst, int frameNum, ref IntPtr[] buf, ref int last, ref IntPtr mhi, Size size, ref double lastTime)
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
                if (r.Height * r.Width > Global.minCarArea && r.Height * r.Width < Global.maxCarArea)
                {
                    MCvScalar s;

                    CarNode node = new CarNode(); ;
                    if (Global.carList.Count == 0)
                    {
                        CarNodeOperation.insertEventNode(ref Global.carList, r, frameNum);
                    }
                    else
                    {
                        node = CarNodeOperation.searchEventList(ref Global.carList, r);
                        if (node == null)
                            node = CarNodeOperation.insertEventNode(ref Global.carList, r, frameNum);
                    }
                    s = CarNodeOperation.sampleColor[node.startFrame % 5];

                    CvInvoke.cvRectangle(img, new Point(r.X, r.Y), new Point(r.X + r.Width, r.Y + r.Height), s, 1, Emgu.CV.CvEnum.LINE_TYPE.CV_AA, 0);
                }
            }

            CarNodeOperation.bianliEventList(ref Global.carList, frameNum);

            CvInvoke.cvReleaseMemStorage(ref stor);
            CvInvoke.cvReleaseImage(ref pyr);

        }

        //分析视频
        static public void carCount(string filePath, VideoMainForm form)
        {
            int N = 3;
            IntPtr capture = CvInvoke.cvCreateFileCapture(filePath);
            IntPtr motion = new IntPtr();
            IntPtr[] buf;
            IntPtr mhi;
            int last = 0;
            double lastTime = 0;

            if (capture.ToInt32() != 0)
            {
                Size captureSize = new Size((int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_WIDTH),
                    (int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_HEIGHT));
                int fps = (int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FPS);

                mhi = CvInvoke.cvCreateImage(captureSize, Emgu.CV.CvEnum.IPL_DEPTH.IPL_DEPTH_32F, 1);
                CvInvoke.cvZero(mhi);
                buf = new IntPtr[N];
                for (int i = 0; i < N; i++)
                {
                    CvInvoke.cvReleaseImage(ref buf[i]);
                    buf[i] = CvInvoke.cvCreateImage(captureSize, Emgu.CV.CvEnum.IPL_DEPTH.IPL_DEPTH_8U, 1);
                    CvInvoke.cvZero(buf[i]);
                }

                CvInvoke.cvNamedWindow("carCount");
                int totalFrames = (int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_COUNT);
                IntPtr frame = new IntPtr();
                int frameNum = 0;
                while (true)
                {
                    //VideoMainForm.analyzeProgressBar.Increment((int)100*frameNum/totalFrames);
                    form.carCountBar.Value = (int)100 * frameNum / totalFrames;
                    form.carResultLabel.Text = Global.carCount.ToString();

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
                        CvInvoke.cvShowImage("carCount", frame);
                        CvInvoke.cvWaitKey(10);
                    }
                    frameNum++;
                }
                CvInvoke.cvDestroyWindow("carCount");
            }

            CvInvoke.cvReleaseCapture(ref capture);

        }
    }
}

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
        static void update_mhi(ref IntPtr img, ref IntPtr dst, int frameNum, ref IntPtr buf, ref int last, ref IntPtr mhi, Size size)
        {
            DateTime.Now.ToShortTimeString();
            DateTime dt = DateTime.Now;
            double timestamp = (double)dt.Ticks / 100;
            int i, idx1, idx2;
            IntPtr silh;
            IntPtr pyr = CvInvoke.cvCreateImage(new Size((size.Width & -2) / 2, (size.Height & -2) / 2), Emgu.CV.CvEnum.IPL_DEPTH.IPL_DEPTH_8U, 1);
            IntPtr stor = CvInvoke.cvCreateMemStorage(0);
            //IntPtr cont = CvInvoke.cvCreateSeq(1, sizeof(Emgu.CV.Seq), sizeof(Point), stor);
              //CvInvoke.cvseq
            int aa = sizeof(int);
            
        }

        //分析视频
        static public void analyzeVideo(string filePath)
        {
            IntPtr capture = CvInvoke.cvCreateFileCapture(filePath);
            IntPtr motion = new IntPtr();
            IntPtr buf;
            IntPtr mhi;
            int last = 0;
           
            if (capture.ToInt32() != 0)
            {
                Size captureSize = new Size((int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_WIDTH),
                    (int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_HEIGHT));
                CvInvoke.cvNamedWindow("analyze");
                int totalFrames = (int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_COUNT);
                IntPtr frame = new IntPtr();
                int frameNum = 0;
                while (true)
                {
                    //VideoMainForm.analyzeProgressBar.Increment((int)100*frameNum/totalFrames);
                    VideoMainForm.analyzeProgressBar.Value = (int)100 * frameNum / totalFrames;
                    frame = CvInvoke.cvQueryFrame(capture);
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
                        //update_mhi(ref frame, ref motion, frameNum, ref buf, last, ref mhi, captureSize);
                        CvInvoke.cvShowImage("analyze", frame);
                        CvInvoke.cvWaitKey(10);
                    }

                    if (frame.ToInt32() == 0)
                    {
                        VideoMainForm.analyzeProgressBar.Value = 100;
                        break;
                    }
                    frameNum++;
                }
                CvInvoke.cvDestroyWindow("analyze");
            }

            CvInvoke.cvReleaseCapture(ref capture);
            
        }
    }
}

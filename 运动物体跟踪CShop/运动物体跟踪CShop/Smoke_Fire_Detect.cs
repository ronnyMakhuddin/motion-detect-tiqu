using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.IO;
using Emgu.CV;
using Emgu.CV.Structure;
using Emgu.CV.UI;

namespace 运动物体跟踪CShop
{
    class Smoke_Fire_Detect
    {
        static public void detectFire(VideoMainForm form, string filePath)
        {
            IntPtr capture = CvInvoke.cvCreateFileCapture(filePath);
            if (capture.ToInt32()<=0)
                return;
            int height = (int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_HEIGHT);
            int width = (int)CvInvoke.cvGetCaptureProperty(capture, Emgu.CV.CvEnum.CAP_PROP.CV_CAP_PROP_FRAME_WIDTH);
            IntPtr frame = new IntPtr();
            IntPtr pf1 = new IntPtr();
            IntPtr pf2 = new IntPtr();
            IntPtr pf3 = new IntPtr();
            IntPtr pframemat1 = new IntPtr();
            IntPtr pframemat2 = new IntPtr();
            IntPtr pframemat3 = new IntPtr();
            IntPtr pframemat4 = new IntPtr();
            IntPtr pframemat5 = new IntPtr();
            IntPtr pframemat6 = new IntPtr();
            int num = 1;
            while (true)
            {
                //运动物体检测部分
                frame = CvInvoke.cvQueryFrame(capture);
                if (frame.ToInt32() <= 0)
                    break;
                if (num % 3 == 1)
                {
                    CvInvoke.cvCvtColor(frame, pf1, Emgu.CV.CvEnum.COLOR_CONVERSION.CV_BGR2GRAY);
                    pframemat1 = CvInvoke.cvCreateMat(height, width, Emgu.CV.CvEnum.MAT_DEPTH.CV_32F);
                    CvInvoke.cvConvert(pf1, pframemat1);
                }
                if (num % 3 == 2)
                {
                    CvInvoke.cvCvtColor(frame, pf2, Emgu.CV.CvEnum.COLOR_CONVERSION.CV_BGR2GRAY);
                    pframemat2 = CvInvoke.cvCreateMat(height, width, Emgu.CV.CvEnum.MAT_DEPTH.CV_32F);
                    CvInvoke.cvConvert(pf2, pframemat2);
                }
                if (num % 3 == 0)
                {
                    CvInvoke.cvCvtColor(frame, pf3, Emgu.CV.CvEnum.COLOR_CONVERSION.CV_BGR2GRAY);
                    pframemat3 = CvInvoke.cvCreateMat(height, width, Emgu.CV.CvEnum.MAT_DEPTH.CV_32F);
                    CvInvoke.cvConvert(pf3, pframemat3);
                }
                CvInvoke.cvSub(pframemat1, pframemat2, pframemat4, new IntPtr(0));
                CvInvoke.cvSub(pframemat3, pframemat2, pframemat5, new IntPtr(0));
                CvInvoke.cvMul(pframemat4, pframemat5, pframemat6, 1);
                int num1 = CvInvoke.cvCountNonZero(pframemat6);

                //烟火检测部分
                int snum1 = 0;
                int snum2 = 0;
                for (int i = 0; i < height; i++)
                {
                    for (int j = 0; j < width; j++)
                    {
                        MCvScalar s = CvInvoke.cvGet2D(frame, i, j);
                        double r1 = s.v0 / (s.v0 + s.v1 + s.v2);
                        double g1 = s.v1 / (s.v0 + s.v1 + s.v2);
                        double r2 = s.v1 / (s.v0 + 1);
                        double g2 = s.v2 / (s.v1 + 1);
                        double b2 = s.v2 / (s.v0 + 1);
                        if ((0.3043 < r1 && r1 < 0.3353) && (0.3187 < g1 && g1< 0.3373) && (r1 < g1))
                            snum1++;
                        if ((0.25 < r2 && r2< 0.65) && (0.2 < g2 && g2< 0.6) && (0.05 < b2 && b2 < 0.45) && (s.v0 > 200) && (s.v2 < 100) && (s.v1 < 200))
                            snum2++;
                    }
                }
                if ((3 <= num1 && num1 <= 25 && snum1 > 10) || (3 <= num1 && num1 <= 25 && snum2 > 10))
                {
                    //检测结果为烟火
                    form.analyzeFireResultLabelSetText("发现烟火");
                }
                else
                {
                    form.analyzeFireResultLabelSetText("没发现烟火");
                }
            }
        }
    }
}

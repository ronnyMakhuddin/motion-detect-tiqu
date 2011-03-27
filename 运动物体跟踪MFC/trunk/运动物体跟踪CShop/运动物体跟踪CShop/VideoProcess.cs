using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Emgu.CV;
using Emgu.CV.Structure;
using Emgu.CV.UI;
namespace 运动物体跟踪CShop
{
    class VideoProcess
    {
        //分析视频
        static public void analyzeVideo(string filePath)
        {
            IntPtr capture = new IntPtr();
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "avi文件|*.avi|所有文件|*.*";
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                capture = CvInvoke.cvCreateFileCapture(openFileDialog.FileName);
            }
            
            IntPtr frame = new IntPtr();
            CvInvoke.cvNamedWindow("video");
            while (true)
            {
                frame = CvInvoke.cvQueryFrame(capture);
                CvInvoke.cvShowImage("video", frame);
                if (CvInvoke.cvWaitKey(50) == 13)
                    break;
            }
            CvInvoke.cvReleaseCapture(ref capture);
            CvInvoke.cvDestroyWindow("video");
        }
    }
}

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Emgu.CV;
using Emgu.CV.Structure;
using Emgu.CV.UI;

namespace 运动物体跟踪CShop
{
    public partial class VideoMainForm : Form
    {
        public VideoMainForm()
        {
            InitializeComponent();
            minAreaTextBox.Text = Global.minArea.ToString();
            maxAreaTextBox.Text = Global.maxArea.ToString();
            jiangeTextBox.Text = Global.jiange.ToString();
        }

        //打开视频文件按钮事件响应
        private void openFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "avi文件|*.avi|所有文件|*.*";
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                Global.filePath = openFileDialog.FileName;
                Global.fileName = openFileDialog.SafeFileName;
                filePathLable.Text = "文件路径:" + Global.filePath;
                Global.eventList.Clear();
            }
        }

        private void VideoMainForm_Load(object sender, EventArgs e)
        {

        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        //分析视频按钮事件响应
        private void analyzeButton_Click(object sender, EventArgs e)
        {
            //VideoProcess.analyzeVideo(Global.filePath);
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "AVI文件|*.avi";
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                CvInvoke.cvNamedWindow("打开视频");
                IntPtr capture = CvInvoke.cvCreateFileCapture(openFileDialog.FileName);
                //Emgu.CV.Capture aa = new Emgu.CV.Capture(openFileDialog.FileName);
                //IntPtr capture = CvInvoke.cvCreateCameraCapture(-1);
                //this.video = capture;
                IntPtr frame = new IntPtr();
                while (true)
                {
                    frame = CvInvoke.cvQueryFrame(capture);
                    CvInvoke.cvShowImage("打开视频", frame);
                    //if (!frame) break;
                    int c = CvInvoke.cvWaitKey(20);
                    if (c == 13) break;
                }
                //this.video = capture;
                CvInvoke.cvWaitKey(0);
                CvInvoke.cvReleaseCapture(ref capture);
                CvInvoke.cvDestroyWindow("打开视频");
            }
        }

        //播放单个事件按钮事件响应
        private void playSingleEventButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "JPG文件|*.jpg|JPEG文件|*.jpeg|所有文件|*.*";
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                CvInvoke.cvNamedWindow("打开图片");
                IntPtr img = CvInvoke.cvLoadImage(openFileDialog.FileName, Emgu.CV.CvEnum.LOAD_IMAGE_TYPE.CV_LOAD_IMAGE_UNCHANGED);
                CvInvoke.cvShowImage("打开图片", img);
                //this.img = img;
                CvInvoke.cvWaitKey(0);
                CvInvoke.cvReleaseImage(ref img);
                CvInvoke.cvDestroyWindow("打开图片");
            }
        }

        //播放所有事件按钮事件响应
        private void playAllEventButton_Click(object sender, EventArgs e)
        {

        }
    }
}

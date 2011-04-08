using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;

using Emgu.CV;
using Emgu.CV.Structure;
using Emgu.CV.UI;
using Emgu.Util;

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
                eventListBox.Items.Clear();
                Global.eventList.Clear();
            }
        }

        private void VideoMainForm_Load(object sender, EventArgs e)
        {
            
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            
        }

        private void eventListBox_DoubleClick(object sender, EventArgs e)
        {
            VideoProcess.playSingleEvent(eventListBox.SelectedIndex);
        }

        //分析视频按钮事件响应
        private void analyzeButton_Click(object sender, EventArgs e)
        {
            if (Global.filePath == null)
                MessageBox.Show("请先选择一个视频文件");
            else
            {
                if (!File.Exists(Global.fileName + ".txt"))
                {
                    VideoProcess.analyzeVideo(Global.filePath, this);
                    EventNodeOperation.eventFilter(ref Global.eventList);
                    FileOperation.writeToFile(Global.fileName + ".txt");
                }
                else
                {
                    FileOperation.readFromFile(Global.fileName + ".txt");
                }

                for (int i = 0; i < Global.eventList.Count; i++)
                {
                    int startFrame = Global.eventList[i].startFrame;
                    int endFrame = Global.eventList[i].endFrame;
                    int totalFrame = endFrame - startFrame;
                    if (totalFrame > Global.maxEventNum)
                        Global.maxEventNum = totalFrame;
                    if (totalFrame < Global.minEventNum)
                        Global.minEventNum = totalFrame;
                    string str = "事件" + (i + 1).ToString() + ",从" + startFrame.ToString() + "到" + endFrame.ToString();
                    eventListBox.Items.Add(str);
                }
            }
        }

        //播放单个事件按钮事件响应
        private void playSingleEventButton_Click(object sender, EventArgs e)
        {
            if (eventListBox.SelectedIndex < 0)
            {
                MessageBox.Show("没有选中事件");
                return;
            }
            VideoProcess.playSingleEvent(eventListBox.SelectedIndex);
        }

        //播放所有事件按钮事件响应
        private void playAllEventButton_Click(object sender, EventArgs e)
        {
            if (Global.eventList.Count == 0)
            {
                MessageBox.Show("事件列表为空,没有可显示的事件");
                return;
            }
            VideoProcess.playAllEvents();
        }

        private void cameralButton_Click(object sender, EventArgs e)
        {
            VideoProcess.cameralSaveVideo();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void carsFromLocalFile_Click(object sender, EventArgs e)
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

        private void carsFromCamera_Click(object sender, EventArgs e)
        {

        }
    }
}

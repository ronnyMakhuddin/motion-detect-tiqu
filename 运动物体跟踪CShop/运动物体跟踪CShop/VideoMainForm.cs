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
            //minAreaTextBox.Text = Global.minArea.ToString();
            //maxAreaTextBox.Text = Global.maxArea.ToString();
            //visualAngelComboBox.SelectedIndex = 0;
            //jiangeTextBox.Text = Global.jiange.ToString();
            carMinAreaTextBox.Text = Global.minCarArea.ToString();
            carMaxAreaTextBox.Text = Global.maxCarArea.ToString();
        }

        //打开视频文件按钮事件响应
        private void openFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "avi文件|*.avi";
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
            VideoAnalyzeProcess.playSingleEvent(eventListBox.SelectedIndex);
        }

        //分析视频按钮事件响应
        private void analyzeButton_Click(object sender, EventArgs e)
        {
            eventListBox.Items.Clear();
            Global.eventList.Clear();
            //Global.minArea = Convert.ToInt32(minAreaTextBox.Text);
            //Global.maxArea = Convert.ToInt32(maxAreaTextBox.Text);
            if (Global.filePath == null)
                MessageBox.Show("请先选择一个视频文件");
            else
            {
                FileInfo fi = new FileInfo(Global.filePath);
                if (!File.Exists(fi.DirectoryName + "\\analyze\\" + fi.Name + ".txt"))
                {

                    VideoAnalyzeProcess.analyzeVideo(Global.filePath, this);
                    EventNodeOperation.eventFilter(ref Global.eventList);
                    if (!Directory.Exists(fi.DirectoryName + "\\analyze"))
                    {
                        Directory.CreateDirectory(fi.DirectoryName + "\\analyze");
                    }
                    FileOperation.writeToFile(fi.DirectoryName + "\\analyze\\" + fi.Name + ".txt");
                }
                else
                {
                    if (MessageBox.Show("检测到已存在此视频的分析文件，是否要读入？", "此视频曾经分析过", MessageBoxButtons.YesNo) != DialogResult.Yes)
                    {
                        VideoAnalyzeProcess.analyzeVideo(Global.filePath, this);
                        EventNodeOperation.eventFilter(ref Global.eventList);
                        FileOperation.writeToFile(fi.DirectoryName + "\\analyze\\" + fi.Name + ".txt");
                    }
                    else
                    {
                        FileOperation.readFromFile(fi.DirectoryName + "\\analyze\\" + fi.Name + ".txt");
                    }

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

                //显示分析结果
                analyzeResultLabel.Text = "分析结果：共" + Global.eventList.Count.ToString() + "个事件，最大事件为" + Global.maxEventNum.ToString() + "帧";
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
            VideoAnalyzeProcess.playSingleEvent(eventListBox.SelectedIndex);
        }

        //播放所有事件按钮事件响应
        private void playAllEventButton_Click(object sender, EventArgs e)
        {
            if (Global.eventList.Count == 0)
            {
                MessageBox.Show("事件列表为空,没有可显示的事件");
                return;
            }
            VideoAnalyzeProcess.playAllEvents();
        }

        private void cameralButton_Click(object sender, EventArgs e)
        {
            VideoAnalyzeProcess.cameralSaveVideo();
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
                Global.carFilePath = openFileDialog.FileName;
                Global.carFileName = openFileDialog.SafeFileName;
                Global.carList.Clear();
                carVideoPathLabel.Text = "文件路径：" + Global.carFilePath;
            }
        }

        private void carsFromCamera_Click(object sender, EventArgs e)
        {

        }

        private void carResultLabel_Click(object sender, EventArgs e)
        {

        }

        private void carCountButton_Click(object sender, EventArgs e)
        {
            Global.carList.Clear();
            Global.minCarArea = Convert.ToInt32(carMinAreaTextBox.Text);
            Global.maxCarArea = Convert.ToInt32(carMaxAreaTextBox.Text);
            CarCountProcess.carCount(Global.carFilePath, this);
            Global.carCount = 0;
        }

        //选择文件夹按钮
        private void batchSecectButton_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderDialog = new FolderBrowserDialog();
            folderDialog.RootFolder = Environment.SpecialFolder.MyComputer;
            if (folderDialog.ShowDialog() == DialogResult.OK)
            {
                filePathLable.Text = "文件路径：" + folderDialog.SelectedPath;
                string[] allFileNames = Directory.GetFiles(folderDialog.SelectedPath);
                List<string> fileNamesList = new List<string>();
                for (int i = 0; i < allFileNames.Length; i++)
                {
                    FileInfo fi = new FileInfo(allFileNames[i]);
                    if (fi.Extension.ToLower() == ".avi")
                    {
                        fileNamesList.Add(allFileNames[i]);
                    }
                }
                FileInfo file = new FileInfo(allFileNames[0]);
                if(!Directory.Exists(file.DirectoryName + "\\analyze"))
                {
                    Directory.CreateDirectory(file.DirectoryName + "\\analyze");
                }
                VideoAnalyzeProcess.batchProcess(fileNamesList, this);
            }
        }

    }
}

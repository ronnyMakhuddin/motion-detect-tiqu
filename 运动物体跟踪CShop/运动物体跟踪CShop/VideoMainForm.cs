using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Threading;

using Emgu.CV;
using Emgu.CV.Structure;
using Emgu.CV.UI;
using Emgu.Util;

namespace 运动物体跟踪CShop
{
    public partial class VideoMainForm : Form
    {
        delegate void SetTextCallback(string text);

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
            if (eventListBox.SelectedIndex < 0)
            {
                MessageBox.Show("没有选中事件");
                return;
            }
            VideoAnalyzeProcess.playSingleEvent(eventListBox.SelectedIndex);
        }

        //分析视频按钮事件响应
        private void analyzeButton_Click(object sender, EventArgs e)
        {
            Thread analyzeSingleVideoThread = new Thread(new ThreadStart(analyzeSigleVideo));
            analyzeSingleVideoThread.Start();
        }

        //分析单个视频线程函数
        public void analyzeSigleVideo()
        {
            eventListBoxClearItem(null);
            Global.eventList.Clear();
            //Global.minArea = Convert.ToInt32(minAreaTextBox.Text);
            //Global.maxArea = Convert.ToInt32(maxAreaTextBox.Text);
            if (Global.filePath == null)
                MessageBox.Show("请先选择一个视频文件");
            else
            {
                bool mark = false;
                FileInfo fi = new FileInfo(Global.filePath);
                if (!File.Exists(fi.DirectoryName + "\\analyze\\" + fi.Name + ".txt"))
                {

                    VideoAnalyzeProcess.analyzeVideo(Global.filePath, this);
                    mark = true;
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
                        mark = true;
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
                    string str = "事件" + (i + 1).ToString() + ",从" + Global.getTimeString(Global.fps, startFrame) + "到" + Global.getTimeString(Global.fps, endFrame);
                    eventListBoxAddItem(str);
                }

                //显示分析结果
                analyzeResultLabelSetText("分析结果：共" + Global.eventList.Count.ToString() + "个事件，最大事件为" + (Global.maxEventNum / Global.fps + 1).ToString() + "秒");
                //生成所有视频
                if (mark)
                {
                    //VideoAnalyzeProcess.playAllEvents(Global.filePath);
                    VideoAnalyzeProcess.playAllEvents_v2(Global.filePath);
                }
               
            }
        }

        //播放单个事件按钮事件响应
        private void playSingleEventButton_Click(object sender, EventArgs e)
        {
            /*
            if (eventListBox.SelectedIndex < 0)
            {
                MessageBox.Show("没有选中事件");
                return;
            }
            VideoAnalyzeProcess.playSingleEvent(eventListBox.SelectedIndex);
            */

            if (eventListBox.SelectedIndex < 0)
            {
                MessageBox.Show("没有选中事件");
                return;
            }
            EventNode eventNode = Global.eventList[eventListBox.SelectedIndex];

            PlayForm playForm = new PlayForm();
            playForm.wmPlayer.URL = Global.filePath;

            playForm.wmPlayer.Ctlcontrols.currentPosition = eventNode.startFrame / Global.fps -1 ;
            playForm.Show();
            //playForm.Hide();
        }

        //播放所有事件按钮事件响应
        private void playAllEventButton_Click(object sender, EventArgs e)
        {
            if (eventListBox.Items.Count == 0)
            {
                MessageBox.Show("事件列表为空,没有可显示的事件");
                return;
            }

            /** 旧版本的播放所有事件
            VideoAnalyzeProcess.playAllEvents();
             * */

            //调用控件来播放所有事件
            PlayForm playForm = new PlayForm();
            FileInfo file = new FileInfo(Global.filePath);

            if (!File.Exists(file.DirectoryName + "\\analyze\\" + file.Name))
            {
                MessageBox.Show("找不到所有事件的视频文件");
                return;
            }

            playForm.wmPlayer.URL = file.DirectoryName + "\\analyze\\" + file.Name;
            playForm.Show();
        }

        private void cameralButton_Click(object sender, EventArgs e)
        {
            //VideoAnalyzeProcess.cameralSaveVideo();
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "avi文件|*.avi";
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                Global.filePath = openFileDialog.FileName;
                Global.fileName = openFileDialog.SafeFileName;
                filePathLable.Text = "文件路径:" + Global.filePath;
                Smoke_Fire_Detect.detectFire(Global.filePath);
            }
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

        List<string> fileNamesList;
        //选择文件夹按钮
        private void batchSecectButton_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog folderDialog = new FolderBrowserDialog();
            folderDialog.RootFolder = Environment.SpecialFolder.MyComputer;
            if (folderDialog.ShowDialog() == DialogResult.OK)
            {
                eventListBox.Items.Clear();
                Global.eventList.Clear();

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

                this.fileNamesList = fileNamesList;
                Thread batchThread = new Thread(new ThreadStart(threadBathProcess));
                batchThread.Start();

                //VideoAnalyzeProcess.batchProcess(fileNamesList, this);
            }
        }

        //线程批处理
        public void threadBathProcess()
        {
            VideoAnalyzeProcess.batchProcess(fileNamesList, this);
        }

        //在线程中设置结果框的值
        public void analyzeResultLabelSetText(string text)
        {
            // InvokeRequired需要比较调用线程ID和创建线程ID
            // 如果它们不相同则返回true
            if (this.analyzeResultLabel.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(analyzeResultLabelSetText);
                this.Invoke(d, new object[] { text });
            }
            else
            {
                this.analyzeResultLabel.Text = text;
                this.analyzeResultLabel.Refresh();
            }
        }
        
        //在线程中设置进度条的值
        public void analyzeProgressBarSetValue(string value)
        {
            // InvokeRequired需要比较调用线程ID和创建线程ID
            // 如果它们不相同则返回true
            if (this.analyzeProgressBar.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(analyzeProgressBarSetValue);
                this.Invoke(d, new object[] { value });
            }
            else
            {
                this.analyzeProgressBar.Value = Convert.ToInt32(value);
                this.analyzeProgressBar.PerformStep();
            }
        }

        //在线程中设置进度条的最大值
        public void analyzeProgressBarSetMaxValue(string maxValue)
        {
            // InvokeRequired需要比较调用线程ID和创建线程ID
            // 如果它们不相同则返回true
            if (this.analyzeProgressBar.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(analyzeProgressBarSetMaxValue);
                this.Invoke(d, new object[] { maxValue });
            }
            else
            {
                this.analyzeProgressBar.Maximum = Convert.ToInt32(maxValue);
                this.analyzeProgressBar.PerformStep();
            }
        }

        //在线程中添加eventListBox的Item
        public void eventListBoxAddItem(string item)
        {
            if (this.eventListBox.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(eventListBoxAddItem);
                this.Invoke(d, new object[] { item });
            }
            else
            {
                this.eventListBox.Items.Add(item);
            }
        }

        //在线程中删除eventListBox所有的Item
        public void eventListBoxClearItem(string text)
        {
            if (this.eventListBox.InvokeRequired)
            {
                SetTextCallback d = new SetTextCallback(eventListBoxClearItem);
                this.Invoke(d, new object[]{text});
                
            }
            else
            {
                this.eventListBox.Items.Clear();
            }
        }

    }
}

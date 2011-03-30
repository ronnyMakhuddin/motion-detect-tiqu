namespace 运动物体跟踪CShop
{
    partial class VideoMainForm
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.eventListBox = new System.Windows.Forms.ListBox();
            this.playAllEventButton = new System.Windows.Forms.Button();
            this.playSingleEventButton = new System.Windows.Forms.Button();
            this.analyzeResultLabel = new System.Windows.Forms.Label();
            this.analyzeProgressBar = new System.Windows.Forms.ProgressBar();
            this.label1 = new System.Windows.Forms.Label();
            this.jiangeTextBox = new System.Windows.Forms.TextBox();
            this.meiLabel = new System.Windows.Forms.Label();
            this.analyzeButton = new System.Windows.Forms.Button();
            this.maxAreaTextBox = new System.Windows.Forms.TextBox();
            this.daoLabel = new System.Windows.Forms.Label();
            this.minAreaTextBox = new System.Windows.Forms.TextBox();
            this.areaLable = new System.Windows.Forms.Label();
            this.filePathLable = new System.Windows.Forms.Label();
            this.openFile = new System.Windows.Forms.Button();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(1, 1);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(670, 450);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.Color.Gainsboro;
            this.tabPage1.Controls.Add(this.eventListBox);
            this.tabPage1.Controls.Add(this.playAllEventButton);
            this.tabPage1.Controls.Add(this.playSingleEventButton);
            this.tabPage1.Controls.Add(this.analyzeResultLabel);
            this.tabPage1.Controls.Add(this.analyzeProgressBar);
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Controls.Add(this.jiangeTextBox);
            this.tabPage1.Controls.Add(this.meiLabel);
            this.tabPage1.Controls.Add(this.analyzeButton);
            this.tabPage1.Controls.Add(this.maxAreaTextBox);
            this.tabPage1.Controls.Add(this.daoLabel);
            this.tabPage1.Controls.Add(this.minAreaTextBox);
            this.tabPage1.Controls.Add(this.areaLable);
            this.tabPage1.Controls.Add(this.filePathLable);
            this.tabPage1.Controls.Add(this.openFile);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(662, 424);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "视频摘要";
            // 
            // eventListBox
            // 
            this.eventListBox.FormattingEnabled = true;
            this.eventListBox.ItemHeight = 12;
            this.eventListBox.Location = new System.Drawing.Point(373, 9);
            this.eventListBox.Name = "eventListBox";
            this.eventListBox.Size = new System.Drawing.Size(283, 364);
            this.eventListBox.TabIndex = 14;
            this.eventListBox.SelectedIndexChanged += new System.EventHandler(this.listBox1_SelectedIndexChanged);
            this.eventListBox.DoubleClick += new System.EventHandler(this.eventListBox_DoubleClick);
            // 
            // playAllEventButton
            // 
            this.playAllEventButton.Location = new System.Drawing.Point(163, 294);
            this.playAllEventButton.Name = "playAllEventButton";
            this.playAllEventButton.Size = new System.Drawing.Size(100, 30);
            this.playAllEventButton.TabIndex = 13;
            this.playAllEventButton.Text = "播放所有事件";
            this.playAllEventButton.UseVisualStyleBackColor = true;
            this.playAllEventButton.Click += new System.EventHandler(this.playAllEventButton_Click);
            // 
            // playSingleEventButton
            // 
            this.playSingleEventButton.Location = new System.Drawing.Point(30, 294);
            this.playSingleEventButton.Name = "playSingleEventButton";
            this.playSingleEventButton.Size = new System.Drawing.Size(100, 30);
            this.playSingleEventButton.TabIndex = 12;
            this.playSingleEventButton.Text = "播放选中事件";
            this.playSingleEventButton.UseVisualStyleBackColor = true;
            this.playSingleEventButton.Click += new System.EventHandler(this.playSingleEventButton_Click);
            // 
            // analyzeResultLabel
            // 
            this.analyzeResultLabel.AutoSize = true;
            this.analyzeResultLabel.Location = new System.Drawing.Point(28, 257);
            this.analyzeResultLabel.Name = "analyzeResultLabel";
            this.analyzeResultLabel.Size = new System.Drawing.Size(59, 12);
            this.analyzeResultLabel.TabIndex = 11;
            this.analyzeResultLabel.Text = "分析结果:";
            // 
            // analyzeProgressBar
            // 
            this.analyzeProgressBar.Location = new System.Drawing.Point(30, 208);
            this.analyzeProgressBar.Name = "analyzeProgressBar";
            this.analyzeProgressBar.Size = new System.Drawing.Size(300, 25);
            this.analyzeProgressBar.TabIndex = 10;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(210, 171);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(65, 12);
            this.label1.TabIndex = 9;
            this.label1.Text = "帧处理一次";
            // 
            // jiangeTextBox
            // 
            this.jiangeTextBox.Location = new System.Drawing.Point(163, 162);
            this.jiangeTextBox.Name = "jiangeTextBox";
            this.jiangeTextBox.Size = new System.Drawing.Size(41, 21);
            this.jiangeTextBox.TabIndex = 8;
            // 
            // meiLabel
            // 
            this.meiLabel.AutoSize = true;
            this.meiLabel.Location = new System.Drawing.Point(140, 170);
            this.meiLabel.Name = "meiLabel";
            this.meiLabel.Size = new System.Drawing.Size(17, 12);
            this.meiLabel.TabIndex = 7;
            this.meiLabel.Text = "每";
            // 
            // analyzeButton
            // 
            this.analyzeButton.Location = new System.Drawing.Point(30, 153);
            this.analyzeButton.Name = "analyzeButton";
            this.analyzeButton.Size = new System.Drawing.Size(100, 30);
            this.analyzeButton.TabIndex = 6;
            this.analyzeButton.Text = "分析视频";
            this.analyzeButton.UseVisualStyleBackColor = true;
            this.analyzeButton.Click += new System.EventHandler(this.analyzeButton_Click);
            // 
            // maxAreaTextBox
            // 
            this.maxAreaTextBox.Location = new System.Drawing.Point(224, 103);
            this.maxAreaTextBox.Name = "maxAreaTextBox";
            this.maxAreaTextBox.Size = new System.Drawing.Size(70, 21);
            this.maxAreaTextBox.TabIndex = 5;
            // 
            // daoLabel
            // 
            this.daoLabel.AutoSize = true;
            this.daoLabel.Location = new System.Drawing.Point(203, 112);
            this.daoLabel.Name = "daoLabel";
            this.daoLabel.Size = new System.Drawing.Size(17, 12);
            this.daoLabel.TabIndex = 4;
            this.daoLabel.Text = "到";
            // 
            // minAreaTextBox
            // 
            this.minAreaTextBox.Location = new System.Drawing.Point(130, 103);
            this.minAreaTextBox.Name = "minAreaTextBox";
            this.minAreaTextBox.Size = new System.Drawing.Size(70, 21);
            this.minAreaTextBox.TabIndex = 3;
            // 
            // areaLable
            // 
            this.areaLable.AutoSize = true;
            this.areaLable.Location = new System.Drawing.Point(28, 112);
            this.areaLable.Name = "areaLable";
            this.areaLable.Size = new System.Drawing.Size(95, 12);
            this.areaLable.TabIndex = 2;
            this.areaLable.Text = "感兴趣面积范围:";
            // 
            // filePathLable
            // 
            this.filePathLable.AutoSize = true;
            this.filePathLable.Location = new System.Drawing.Point(28, 81);
            this.filePathLable.Name = "filePathLable";
            this.filePathLable.Size = new System.Drawing.Size(59, 12);
            this.filePathLable.TabIndex = 1;
            this.filePathLable.Text = "文件路径:";
            // 
            // openFile
            // 
            this.openFile.Location = new System.Drawing.Point(30, 30);
            this.openFile.Name = "openFile";
            this.openFile.Size = new System.Drawing.Size(100, 30);
            this.openFile.TabIndex = 0;
            this.openFile.Text = "选择视频文件";
            this.openFile.UseVisualStyleBackColor = true;
            this.openFile.Click += new System.EventHandler(this.openFile_Click);
            // 
            // tabPage2
            // 
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(662, 424);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "tabPage2";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // VideoMainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(671, 408);
            this.Controls.Add(this.tabControl1);
            this.Name = "VideoMainForm";
            this.Text = "视频摘要软件";
            this.Load += new System.EventHandler(this.VideoMainForm_Load);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Button openFile;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Label filePathLable;
        private System.Windows.Forms.Label areaLable;
        private System.Windows.Forms.Button analyzeButton;
        private System.Windows.Forms.TextBox maxAreaTextBox;
        private System.Windows.Forms.Label daoLabel;
        private System.Windows.Forms.TextBox minAreaTextBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox jiangeTextBox;
        private System.Windows.Forms.Label meiLabel;
        private System.Windows.Forms.Label analyzeResultLabel;
        private System.Windows.Forms.Button playSingleEventButton;
        private System.Windows.Forms.ListBox eventListBox;
        private System.Windows.Forms.Button playAllEventButton;
        public System.Windows.Forms.ProgressBar analyzeProgressBar;
    }
}


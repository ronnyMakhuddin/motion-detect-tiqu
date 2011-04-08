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
            this.analyzeResultLabel = new System.Windows.Forms.Label();
            this.analyzeProgressBar = new System.Windows.Forms.ProgressBar();
            this.label1 = new System.Windows.Forms.Label();
            this.jiangeTextBox = new System.Windows.Forms.TextBox();
            this.meiLabel = new System.Windows.Forms.Label();
            this.maxAreaTextBox = new System.Windows.Forms.TextBox();
            this.daoLabel = new System.Windows.Forms.Label();
            this.minAreaTextBox = new System.Windows.Forms.TextBox();
            this.areaLable = new System.Windows.Forms.Label();
            this.filePathLable = new System.Windows.Forms.Label();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.cameralButton = new System.Windows.Forms.Button();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.playAllEventButton = new System.Windows.Forms.Button();
            this.playSingleEventButton = new System.Windows.Forms.Button();
            this.analyzeButton = new System.Windows.Forms.Button();
            this.openFile = new System.Windows.Forms.Button();
            this.carsFromCamera = new System.Windows.Forms.Button();
            this.carsFromLocalFile = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage4);
            this.tabControl1.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.tabControl1.ItemSize = new System.Drawing.Size(0, 50);
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Multiline = true;
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.Padding = new System.Drawing.Point(0, 0);
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(671, 411);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.Color.LightGray;
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
            this.tabPage1.Location = new System.Drawing.Point(4, 54);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(663, 353);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "      视频摘要      ";
            this.tabPage1.ToolTipText = "111";
            // 
            // eventListBox
            // 
            this.eventListBox.FormattingEnabled = true;
            this.eventListBox.ItemHeight = 16;
            this.eventListBox.Location = new System.Drawing.Point(373, 9);
            this.eventListBox.Name = "eventListBox";
            this.eventListBox.Size = new System.Drawing.Size(283, 324);
            this.eventListBox.TabIndex = 14;
            this.eventListBox.SelectedIndexChanged += new System.EventHandler(this.listBox1_SelectedIndexChanged);
            this.eventListBox.DoubleClick += new System.EventHandler(this.eventListBox_DoubleClick);
            // 
            // analyzeResultLabel
            // 
            this.analyzeResultLabel.AutoSize = true;
            this.analyzeResultLabel.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.analyzeResultLabel.Location = new System.Drawing.Point(28, 262);
            this.analyzeResultLabel.Name = "analyzeResultLabel";
            this.analyzeResultLabel.Size = new System.Drawing.Size(70, 14);
            this.analyzeResultLabel.TabIndex = 11;
            this.analyzeResultLabel.Text = "分析结果:";
            // 
            // analyzeProgressBar
            // 
            this.analyzeProgressBar.Location = new System.Drawing.Point(30, 221);
            this.analyzeProgressBar.Name = "analyzeProgressBar";
            this.analyzeProgressBar.Size = new System.Drawing.Size(300, 25);
            this.analyzeProgressBar.TabIndex = 10;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.label1.Location = new System.Drawing.Point(241, 170);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 14);
            this.label1.TabIndex = 9;
            this.label1.Text = "帧处理一次";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // jiangeTextBox
            // 
            this.jiangeTextBox.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.jiangeTextBox.Location = new System.Drawing.Point(194, 165);
            this.jiangeTextBox.Name = "jiangeTextBox";
            this.jiangeTextBox.Size = new System.Drawing.Size(41, 23);
            this.jiangeTextBox.TabIndex = 8;
            // 
            // meiLabel
            // 
            this.meiLabel.AutoSize = true;
            this.meiLabel.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.meiLabel.Location = new System.Drawing.Point(167, 170);
            this.meiLabel.Name = "meiLabel";
            this.meiLabel.Size = new System.Drawing.Size(21, 14);
            this.meiLabel.TabIndex = 7;
            this.meiLabel.Text = "每";
            // 
            // maxAreaTextBox
            // 
            this.maxAreaTextBox.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.maxAreaTextBox.Location = new System.Drawing.Point(249, 105);
            this.maxAreaTextBox.Name = "maxAreaTextBox";
            this.maxAreaTextBox.Size = new System.Drawing.Size(70, 23);
            this.maxAreaTextBox.TabIndex = 5;
            // 
            // daoLabel
            // 
            this.daoLabel.AutoSize = true;
            this.daoLabel.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.daoLabel.Location = new System.Drawing.Point(222, 110);
            this.daoLabel.Name = "daoLabel";
            this.daoLabel.Size = new System.Drawing.Size(21, 14);
            this.daoLabel.TabIndex = 4;
            this.daoLabel.Text = "到";
            // 
            // minAreaTextBox
            // 
            this.minAreaTextBox.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.minAreaTextBox.Location = new System.Drawing.Point(146, 105);
            this.minAreaTextBox.Name = "minAreaTextBox";
            this.minAreaTextBox.Size = new System.Drawing.Size(70, 23);
            this.minAreaTextBox.TabIndex = 3;
            // 
            // areaLable
            // 
            this.areaLable.AutoSize = true;
            this.areaLable.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.areaLable.Location = new System.Drawing.Point(28, 110);
            this.areaLable.Name = "areaLable";
            this.areaLable.Size = new System.Drawing.Size(112, 14);
            this.areaLable.TabIndex = 2;
            this.areaLable.Text = "感兴趣面积范围:";
            // 
            // filePathLable
            // 
            this.filePathLable.AutoSize = true;
            this.filePathLable.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.filePathLable.Location = new System.Drawing.Point(28, 81);
            this.filePathLable.Name = "filePathLable";
            this.filePathLable.Size = new System.Drawing.Size(70, 14);
            this.filePathLable.TabIndex = 1;
            this.filePathLable.Text = "文件路径:";
            // 
            // tabPage3
            // 
            this.tabPage3.BackColor = System.Drawing.Color.LightGray;
            this.tabPage3.Controls.Add(this.label2);
            this.tabPage3.Controls.Add(this.progressBar1);
            this.tabPage3.Controls.Add(this.carsFromCamera);
            this.tabPage3.Controls.Add(this.carsFromLocalFile);
            this.tabPage3.Location = new System.Drawing.Point(4, 54);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size(663, 353);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "      车流计数    ";
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(152, 139);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(367, 26);
            this.progressBar1.TabIndex = 2;
            // 
            // tabPage2
            // 
            this.tabPage2.BackColor = System.Drawing.Color.LightGray;
            this.tabPage2.Controls.Add(this.cameralButton);
            this.tabPage2.Location = new System.Drawing.Point(4, 54);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(663, 353);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "     摄像头录像     ";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // cameralButton
            // 
            this.cameralButton.Location = new System.Drawing.Point(183, 139);
            this.cameralButton.Name = "cameralButton";
            this.cameralButton.Size = new System.Drawing.Size(121, 60);
            this.cameralButton.TabIndex = 0;
            this.cameralButton.Text = "摄像头保存视频";
            this.cameralButton.UseVisualStyleBackColor = true;
            this.cameralButton.Click += new System.EventHandler(this.cameralButton_Click);
            // 
            // tabPage4
            // 
            this.tabPage4.BackColor = System.Drawing.Color.LightGray;
            this.tabPage4.Location = new System.Drawing.Point(4, 54);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Size = new System.Drawing.Size(663, 353);
            this.tabPage4.TabIndex = 3;
            this.tabPage4.Text = "     软件信息     ";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // playAllEventButton
            // 
            this.playAllEventButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.playAllEventButton.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.playAllEventButton.Image = global::运动物体跟踪CShop.Properties.Resources.playAllEventButton;
            this.playAllEventButton.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.playAllEventButton.Location = new System.Drawing.Point(203, 292);
            this.playAllEventButton.Name = "playAllEventButton";
            this.playAllEventButton.Size = new System.Drawing.Size(152, 50);
            this.playAllEventButton.TabIndex = 13;
            this.playAllEventButton.Text = "播放所有事件";
            this.playAllEventButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.playAllEventButton.UseVisualStyleBackColor = true;
            this.playAllEventButton.Click += new System.EventHandler(this.playAllEventButton_Click);
            // 
            // playSingleEventButton
            // 
            this.playSingleEventButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.playSingleEventButton.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.playSingleEventButton.Image = global::运动物体跟踪CShop.Properties.Resources.playSingleEventButton;
            this.playSingleEventButton.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.playSingleEventButton.Location = new System.Drawing.Point(30, 292);
            this.playSingleEventButton.Name = "playSingleEventButton";
            this.playSingleEventButton.Size = new System.Drawing.Size(152, 50);
            this.playSingleEventButton.TabIndex = 12;
            this.playSingleEventButton.Text = "播放选中事件";
            this.playSingleEventButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.playSingleEventButton.UseVisualStyleBackColor = true;
            this.playSingleEventButton.Click += new System.EventHandler(this.playSingleEventButton_Click);
            // 
            // analyzeButton
            // 
            this.analyzeButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.analyzeButton.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.analyzeButton.Image = global::运动物体跟踪CShop.Properties.Resources.analyzeButton;
            this.analyzeButton.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.analyzeButton.Location = new System.Drawing.Point(30, 151);
            this.analyzeButton.Name = "analyzeButton";
            this.analyzeButton.Size = new System.Drawing.Size(120, 50);
            this.analyzeButton.TabIndex = 6;
            this.analyzeButton.Text = "分析视频";
            this.analyzeButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.analyzeButton.UseVisualStyleBackColor = true;
            this.analyzeButton.Click += new System.EventHandler(this.analyzeButton_Click);
            // 
            // openFile
            // 
            this.openFile.BackColor = System.Drawing.Color.Transparent;
            this.openFile.Cursor = System.Windows.Forms.Cursors.Hand;
            this.openFile.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.openFile.Image = global::运动物体跟踪CShop.Properties.Resources.openFileButton;
            this.openFile.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.openFile.Location = new System.Drawing.Point(30, 18);
            this.openFile.Margin = new System.Windows.Forms.Padding(0);
            this.openFile.Name = "openFile";
            this.openFile.Size = new System.Drawing.Size(152, 50);
            this.openFile.TabIndex = 0;
            this.openFile.Text = "选择视频文件";
            this.openFile.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.openFile.UseVisualStyleBackColor = false;
            this.openFile.Click += new System.EventHandler(this.openFile_Click);
            // 
            // carsFromCamera
            // 
            this.carsFromCamera.Image = global::运动物体跟踪CShop.Properties.Resources.cameral;
            this.carsFromCamera.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.carsFromCamera.Location = new System.Drawing.Point(362, 45);
            this.carsFromCamera.Name = "carsFromCamera";
            this.carsFromCamera.Size = new System.Drawing.Size(157, 50);
            this.carsFromCamera.TabIndex = 1;
            this.carsFromCamera.Text = "摄像头车流";
            this.carsFromCamera.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.carsFromCamera.UseVisualStyleBackColor = true;
            this.carsFromCamera.Click += new System.EventHandler(this.carsFromCamera_Click);
            // 
            // carsFromLocalFile
            // 
            this.carsFromLocalFile.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.carsFromLocalFile.Image = global::运动物体跟踪CShop.Properties.Resources.openFileButton;
            this.carsFromLocalFile.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.carsFromLocalFile.Location = new System.Drawing.Point(152, 45);
            this.carsFromLocalFile.Name = "carsFromLocalFile";
            this.carsFromLocalFile.Size = new System.Drawing.Size(154, 50);
            this.carsFromLocalFile.TabIndex = 0;
            this.carsFromLocalFile.Text = "本地车流视频";
            this.carsFromLocalFile.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.carsFromLocalFile.UseVisualStyleBackColor = true;
            this.carsFromLocalFile.Click += new System.EventHandler(this.carsFromLocalFile_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(149, 202);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(88, 16);
            this.label2.TabIndex = 3;
            this.label2.Text = "车流计数：";
            // 
            // VideoMainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(669, 408);
            this.Controls.Add(this.tabControl1);
            this.MaximizeBox = false;
            this.Name = "VideoMainForm";
            this.Text = "视频摘要软件";
            this.Load += new System.EventHandler(this.VideoMainForm_Load);
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage3.ResumeLayout(false);
            this.tabPage3.PerformLayout();
            this.tabPage2.ResumeLayout(false);
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
        private System.Windows.Forms.Button cameralButton;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.Button carsFromLocalFile;
        private System.Windows.Forms.Button carsFromCamera;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.Label label2;
    }
}


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
            this.label1 = new System.Windows.Forms.Label();
            this.batchSecectButton = new System.Windows.Forms.Button();
            this.eventListBox = new System.Windows.Forms.ListBox();
            this.playAllEventButton = new System.Windows.Forms.Button();
            this.playSingleEventButton = new System.Windows.Forms.Button();
            this.analyzeResultLabel = new System.Windows.Forms.Label();
            this.analyzeProgressBar = new System.Windows.Forms.ProgressBar();
            this.analyzeButton = new System.Windows.Forms.Button();
            this.filePathLable = new System.Windows.Forms.Label();
            this.openFile = new System.Windows.Forms.Button();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.carCountButton = new System.Windows.Forms.Button();
            this.carMaxAreaTextBox = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.carMinAreaTextBox = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.carVideoPathLabel = new System.Windows.Forms.Label();
            this.carResultLabel = new System.Windows.Forms.Label();
            this.carCountBar = new System.Windows.Forms.ProgressBar();
            this.carsFromCamera = new System.Windows.Forms.Button();
            this.carsFromLocalFile = new System.Windows.Forms.Button();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.cameralButton = new System.Windows.Forms.Button();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.fireResult = new System.Windows.Forms.Label();
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
            this.tabPage1.Controls.Add(this.label1);
            this.tabPage1.Controls.Add(this.batchSecectButton);
            this.tabPage1.Controls.Add(this.eventListBox);
            this.tabPage1.Controls.Add(this.playAllEventButton);
            this.tabPage1.Controls.Add(this.playSingleEventButton);
            this.tabPage1.Controls.Add(this.analyzeResultLabel);
            this.tabPage1.Controls.Add(this.analyzeProgressBar);
            this.tabPage1.Controls.Add(this.analyzeButton);
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
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(370, 18);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(88, 16);
            this.label1.TabIndex = 17;
            this.label1.Text = "事件列表：";
            // 
            // batchSecectButton
            // 
            this.batchSecectButton.Image = global::运动物体跟踪CShop.Properties.Resources.openDirectory;
            this.batchSecectButton.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.batchSecectButton.Location = new System.Drawing.Point(188, 18);
            this.batchSecectButton.Name = "batchSecectButton";
            this.batchSecectButton.Size = new System.Drawing.Size(152, 50);
            this.batchSecectButton.TabIndex = 16;
            this.batchSecectButton.Text = "选择文件夹";
            this.batchSecectButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.batchSecectButton.UseVisualStyleBackColor = true;
            this.batchSecectButton.Click += new System.EventHandler(this.batchSecectButton_Click);
            // 
            // eventListBox
            // 
            this.eventListBox.FormattingEnabled = true;
            this.eventListBox.ItemHeight = 16;
            this.eventListBox.Location = new System.Drawing.Point(373, 41);
            this.eventListBox.Name = "eventListBox";
            this.eventListBox.Size = new System.Drawing.Size(283, 292);
            this.eventListBox.TabIndex = 14;
            this.eventListBox.SelectedIndexChanged += new System.EventHandler(this.listBox1_SelectedIndexChanged);
            this.eventListBox.DoubleClick += new System.EventHandler(this.eventListBox_DoubleClick);
            // 
            // playAllEventButton
            // 
            this.playAllEventButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.playAllEventButton.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.playAllEventButton.Image = global::运动物体跟踪CShop.Properties.Resources.playAllEventButton;
            this.playAllEventButton.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.playAllEventButton.Location = new System.Drawing.Point(188, 288);
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
            this.playSingleEventButton.Location = new System.Drawing.Point(15, 288);
            this.playSingleEventButton.Name = "playSingleEventButton";
            this.playSingleEventButton.Size = new System.Drawing.Size(152, 50);
            this.playSingleEventButton.TabIndex = 12;
            this.playSingleEventButton.Text = "播放选中事件";
            this.playSingleEventButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.playSingleEventButton.UseVisualStyleBackColor = true;
            this.playSingleEventButton.Click += new System.EventHandler(this.playSingleEventButton_Click);
            // 
            // analyzeResultLabel
            // 
            this.analyzeResultLabel.AutoSize = true;
            this.analyzeResultLabel.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.analyzeResultLabel.Location = new System.Drawing.Point(13, 251);
            this.analyzeResultLabel.Name = "analyzeResultLabel";
            this.analyzeResultLabel.Size = new System.Drawing.Size(70, 14);
            this.analyzeResultLabel.TabIndex = 11;
            this.analyzeResultLabel.Text = "分析结果:";
            // 
            // analyzeProgressBar
            // 
            this.analyzeProgressBar.Location = new System.Drawing.Point(15, 201);
            this.analyzeProgressBar.Name = "analyzeProgressBar";
            this.analyzeProgressBar.Size = new System.Drawing.Size(254, 25);
            this.analyzeProgressBar.TabIndex = 10;
            // 
            // analyzeButton
            // 
            this.analyzeButton.Cursor = System.Windows.Forms.Cursors.Hand;
            this.analyzeButton.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.analyzeButton.Image = global::运动物体跟踪CShop.Properties.Resources.analyzeButton;
            this.analyzeButton.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.analyzeButton.Location = new System.Drawing.Point(15, 125);
            this.analyzeButton.Name = "analyzeButton";
            this.analyzeButton.Size = new System.Drawing.Size(152, 50);
            this.analyzeButton.TabIndex = 6;
            this.analyzeButton.Text = "分析视频";
            this.analyzeButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.analyzeButton.UseVisualStyleBackColor = true;
            this.analyzeButton.Click += new System.EventHandler(this.analyzeButton_Click);
            // 
            // filePathLable
            // 
            this.filePathLable.AutoSize = true;
            this.filePathLable.Font = new System.Drawing.Font("宋体", 10.5F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.filePathLable.Location = new System.Drawing.Point(13, 85);
            this.filePathLable.Name = "filePathLable";
            this.filePathLable.Size = new System.Drawing.Size(70, 14);
            this.filePathLable.TabIndex = 1;
            this.filePathLable.Text = "文件路径:";
            // 
            // openFile
            // 
            this.openFile.BackColor = System.Drawing.Color.Transparent;
            this.openFile.Cursor = System.Windows.Forms.Cursors.Hand;
            this.openFile.Font = new System.Drawing.Font("宋体", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.openFile.Image = global::运动物体跟踪CShop.Properties.Resources.openFileButton;
            this.openFile.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.openFile.Location = new System.Drawing.Point(15, 18);
            this.openFile.Margin = new System.Windows.Forms.Padding(0);
            this.openFile.Name = "openFile";
            this.openFile.Size = new System.Drawing.Size(152, 50);
            this.openFile.TabIndex = 0;
            this.openFile.Text = "选择视频文件";
            this.openFile.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.openFile.UseVisualStyleBackColor = false;
            this.openFile.Click += new System.EventHandler(this.openFile_Click);
            // 
            // tabPage3
            // 
            this.tabPage3.BackColor = System.Drawing.Color.LightGray;
            this.tabPage3.Controls.Add(this.carCountButton);
            this.tabPage3.Controls.Add(this.carMaxAreaTextBox);
            this.tabPage3.Controls.Add(this.label3);
            this.tabPage3.Controls.Add(this.carMinAreaTextBox);
            this.tabPage3.Controls.Add(this.label2);
            this.tabPage3.Controls.Add(this.carVideoPathLabel);
            this.tabPage3.Controls.Add(this.carResultLabel);
            this.tabPage3.Controls.Add(this.carCountBar);
            this.tabPage3.Controls.Add(this.carsFromCamera);
            this.tabPage3.Controls.Add(this.carsFromLocalFile);
            this.tabPage3.Location = new System.Drawing.Point(4, 54);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size(663, 353);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "      车流计数    ";
            // 
            // carCountButton
            // 
            this.carCountButton.Image = global::运动物体跟踪CShop.Properties.Resources.jishu;
            this.carCountButton.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.carCountButton.Location = new System.Drawing.Point(141, 175);
            this.carCountButton.Name = "carCountButton";
            this.carCountButton.Size = new System.Drawing.Size(136, 50);
            this.carCountButton.TabIndex = 9;
            this.carCountButton.Text = "车流计数";
            this.carCountButton.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.carCountButton.UseVisualStyleBackColor = true;
            this.carCountButton.Click += new System.EventHandler(this.carCountButton_Click);
            // 
            // carMaxAreaTextBox
            // 
            this.carMaxAreaTextBox.Location = new System.Drawing.Point(362, 130);
            this.carMaxAreaTextBox.Name = "carMaxAreaTextBox";
            this.carMaxAreaTextBox.Size = new System.Drawing.Size(78, 26);
            this.carMaxAreaTextBox.TabIndex = 8;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(334, 133);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(24, 16);
            this.label3.TabIndex = 7;
            this.label3.Text = "到";
            // 
            // carMinAreaTextBox
            // 
            this.carMinAreaTextBox.Location = new System.Drawing.Point(250, 130);
            this.carMinAreaTextBox.Name = "carMinAreaTextBox";
            this.carMinAreaTextBox.Size = new System.Drawing.Size(78, 26);
            this.carMinAreaTextBox.TabIndex = 6;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(138, 133);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(120, 16);
            this.label2.TabIndex = 5;
            this.label2.Text = "车辆面积范围：";
            // 
            // carVideoPathLabel
            // 
            this.carVideoPathLabel.AutoSize = true;
            this.carVideoPathLabel.Location = new System.Drawing.Point(138, 88);
            this.carVideoPathLabel.Name = "carVideoPathLabel";
            this.carVideoPathLabel.Size = new System.Drawing.Size(88, 16);
            this.carVideoPathLabel.TabIndex = 4;
            this.carVideoPathLabel.Text = "文件路径：";
            // 
            // carResultLabel
            // 
            this.carResultLabel.AutoSize = true;
            this.carResultLabel.Location = new System.Drawing.Point(138, 297);
            this.carResultLabel.Name = "carResultLabel";
            this.carResultLabel.Size = new System.Drawing.Size(88, 16);
            this.carResultLabel.TabIndex = 3;
            this.carResultLabel.Text = "车流计数：";
            this.carResultLabel.Click += new System.EventHandler(this.carResultLabel_Click);
            // 
            // carCountBar
            // 
            this.carCountBar.Location = new System.Drawing.Point(141, 245);
            this.carCountBar.Name = "carCountBar";
            this.carCountBar.Size = new System.Drawing.Size(367, 26);
            this.carCountBar.TabIndex = 2;
            // 
            // carsFromCamera
            // 
            this.carsFromCamera.Cursor = System.Windows.Forms.Cursors.Hand;
            this.carsFromCamera.Image = global::运动物体跟踪CShop.Properties.Resources.cameral;
            this.carsFromCamera.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.carsFromCamera.Location = new System.Drawing.Point(362, 17);
            this.carsFromCamera.Name = "carsFromCamera";
            this.carsFromCamera.Size = new System.Drawing.Size(146, 50);
            this.carsFromCamera.TabIndex = 1;
            this.carsFromCamera.Text = "摄像头车流";
            this.carsFromCamera.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.carsFromCamera.UseVisualStyleBackColor = true;
            this.carsFromCamera.Click += new System.EventHandler(this.carsFromCamera_Click);
            // 
            // carsFromLocalFile
            // 
            this.carsFromLocalFile.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.carsFromLocalFile.Cursor = System.Windows.Forms.Cursors.Hand;
            this.carsFromLocalFile.Image = global::运动物体跟踪CShop.Properties.Resources.openFileButton;
            this.carsFromLocalFile.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.carsFromLocalFile.Location = new System.Drawing.Point(141, 17);
            this.carsFromLocalFile.Name = "carsFromLocalFile";
            this.carsFromLocalFile.Size = new System.Drawing.Size(154, 50);
            this.carsFromLocalFile.TabIndex = 0;
            this.carsFromLocalFile.Text = "本地车流视频";
            this.carsFromLocalFile.TextImageRelation = System.Windows.Forms.TextImageRelation.ImageBeforeText;
            this.carsFromLocalFile.UseVisualStyleBackColor = true;
            this.carsFromLocalFile.Click += new System.EventHandler(this.carsFromLocalFile_Click);
            // 
            // tabPage2
            // 
            this.tabPage2.BackColor = System.Drawing.Color.LightGray;
            this.tabPage2.Controls.Add(this.fireResult);
            this.tabPage2.Controls.Add(this.cameralButton);
            this.tabPage2.Location = new System.Drawing.Point(4, 54);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(663, 353);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "      烟火检测      ";
            // 
            // cameralButton
            // 
            this.cameralButton.Location = new System.Drawing.Point(239, 60);
            this.cameralButton.Name = "cameralButton";
            this.cameralButton.Size = new System.Drawing.Size(121, 60);
            this.cameralButton.TabIndex = 0;
            this.cameralButton.Text = "读入视频文件";
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
            // fireResult
            // 
            this.fireResult.AutoSize = true;
            this.fireResult.Location = new System.Drawing.Point(273, 222);
            this.fireResult.Name = "fireResult";
            this.fireResult.Size = new System.Drawing.Size(56, 16);
            this.fireResult.TabIndex = 1;
            this.fireResult.Text = "无烟火";
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
            this.tabPage2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Button openFile;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Label filePathLable;
        private System.Windows.Forms.Button analyzeButton;
        private System.Windows.Forms.Button playSingleEventButton;
        private System.Windows.Forms.ListBox eventListBox;
        private System.Windows.Forms.Button playAllEventButton;
        public System.Windows.Forms.ProgressBar analyzeProgressBar;
        private System.Windows.Forms.Button cameralButton;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.Button carsFromLocalFile;
        private System.Windows.Forms.Button carsFromCamera;
        public System.Windows.Forms.ProgressBar carCountBar;
        public System.Windows.Forms.Label carResultLabel;
        private System.Windows.Forms.Label carVideoPathLabel;
        private System.Windows.Forms.TextBox carMaxAreaTextBox;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox carMinAreaTextBox;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button carCountButton;
        private System.Windows.Forms.Button batchSecectButton;
        public System.Windows.Forms.Label analyzeResultLabel;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label fireResult;
    }
}


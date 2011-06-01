using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace 运动物体跟踪CShop
{
    public partial class PlayForm : Form
    {
        public PlayForm()
        {
            InitializeComponent();
            
            //wmPlayer.URL = "D:\\My Documents\\vs 2008 project\\Num1.avi";
            //wmPlayer.Ctlcontrols.currentPosition = 20.9;
            //wmPlayer.Ctlcontrols.play();
        }

        private void axWindowsMediaPlayer1_Enter(object sender, EventArgs e)
        {

        }

        //当窗口尺寸发生改变的时候响应
        private void SizeChanged_size(object sender, EventArgs e)
        {
            wmPlayer.Size = new Size(this.Size.Width-20,this.Size.Height-40);
        }
        
    }
}

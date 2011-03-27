using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using Emgu.CV;
using Emgu.CV.Structure;
using Emgu.CV.UI;
namespace 运动物体跟踪CShop
{
    class TrackNode
    {
        public Rectangle rect;
        public int frameNum;
    }
}

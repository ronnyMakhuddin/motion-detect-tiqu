using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace 运动物体跟踪CShop
{
    class Global
    {
        static public string filePath;
        static public string fileName;
        static public List<EventNode> eventList = new List<EventNode> ();
        static public int minArea = 1000;
        static public int maxArea = 60000;
        static public int jiange = 2;
        static public int maxEventNum = 0;
        static public int minEventNum = 10000000;
    }
}

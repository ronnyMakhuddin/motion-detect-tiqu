using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace 运动物体跟踪CShop
{
    class Global
    {
        static public string filePath = null;
        static public string fileName  = null;
        static public string carFilePath = null;
        static public string carFileName = null;
        static public List<EventNode> eventList = new List<EventNode> ();
        static public List<CarNode> carList = new List<CarNode>();
        static public int fps = 0;
        static public int carCount = 0;
        static public int minArea = 1000;
        static public int maxArea = 60000;
        static public int minCarArea = 10000;
        static public int maxCarArea = 600000;
        static public int jiange = 4;
        static public int maxEventNum = 0;
        static public int minEventNum = 10000000;
        static public int LIMIT = 100;
        static public PlayForm playForm = new PlayForm();

        static public string getTimeString(int fps, int frameNum)
        {
            int totalSeconds = frameNum / fps;
            int totalMinutes = totalSeconds / 60;
            int seconds = totalSeconds % 60;
            int minutes = totalMinutes % 60;
            int hours = totalMinutes / 60;
            string time = hours.ToString() + ":" + minutes.ToString() + ":" + seconds.ToString();
            return time;
        }
    }
}

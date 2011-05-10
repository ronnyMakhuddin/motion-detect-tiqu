using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Windows.Forms;
using System.Drawing;

namespace 运动物体跟踪CShop
{
    class FileOperation
    {
        static public void writeToFile(string filePath)
        {
            FileStream fs = new FileStream(filePath, FileMode.OpenOrCreate);
            StreamWriter sw = new StreamWriter(fs);
            sw.Write(Global.eventList.Count.ToString() + " " + Global.jiange.ToString() + " " + Global.fps.ToString());
            sw.Write("\r\n");
            for (int i = 0; i < Global.eventList.Count; i++)
            {
                EventNode node = Global.eventList[i];
                int startFrame = node.startFrame;
                int endFrame = node.endFrame;
                sw.Write(startFrame.ToString() + " " + endFrame.ToString() + " " + node.trackList.Count.ToString() + " ");
                for (int j = 0; j < node.trackList.Count; j++)
                {
                    int x = node.trackList[j].X;
                    int y = node.trackList[j].Y;
                    int width = node.trackList[j].Width;
                    int height = node.trackList[j].Height;
                    sw.Write(x.ToString() + " " + y.ToString() + " " + width.ToString() + " " + height.ToString() + " ");
                }
                sw.Write("\r\n");
            }
            sw.Flush();
            sw.Close();
            fs.Close();
        }

        static public void readFromFile(string filePath)
        {
            int N = 3;
            StreamReader fileReader = new StreamReader(filePath);
            string head = fileReader.ReadLine();
            string[] temp = head.Split(' ');
            int total = Convert.ToInt32(temp[0]);
            Global.jiange = Convert.ToInt32(temp[1]);
            Global.fps = Convert.ToInt32(temp[2]);
            Global.eventList.Clear();
            for (int i = 0; i < total; i++)
            {
                EventNode node = new EventNode();
                node.trackList = new List<Rectangle>();
                string line = fileReader.ReadLine();
                string [] data = line.Split(' ');
                node.startFrame = Convert.ToInt32(data[0]);
                node.endFrame = Convert.ToInt32(data[1]);
                int trackNum = Convert.ToInt32(data[2]);
                for (int j = 0; j < trackNum; j++)
                {
                    int x = Convert.ToInt32(data[j * 4 + N]);
                    int y = Convert.ToInt32(data[j * 4 + N + 1]);
                    int width = Convert.ToInt32(data[j * 4 + N + 2]);
                    int height = Convert.ToInt32(data[j * 4 + N + 3]);
                    Rectangle r = new Rectangle(x, y, width, height);
                    node.trackList.Add(r);
                }
                Global.eventList.Add(node);
            }
            fileReader.Close();
        }
    }
}

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
    class EventNode
    {
        public bool mark;
        public int startFrame;
        public int endFrame;
        public Rectangle rect;
        public List<Rectangle> trackList;

        public IntPtr capture;
    }
    

    static class EventNodeOperation
    {
        //static public Bgr[] sampleColor = new Bgr[5] {new Bgr(Color.Red), new Bgr(Color.Blue), new Bgr(Color.Black), new Bgr(Color.Brown), new Bgr(Color.Green) };
        static public MCvScalar[] sampleColor = new MCvScalar[5] { new MCvScalar(0, 0, 0), new MCvScalar(255, 0, 0), new MCvScalar(0, 255, 0), new MCvScalar(0, 0, 255), new MCvScalar(255, 255, 0) };

        //判断是否同一个对象的辅助函数
        static public int findLength(int [] a)
        {
            int minIndex = 0, maxIndex = 0;
            for (int i = 0; i < 4; i++)
            {
                if (a[i] < a[minIndex])
                    minIndex = i;
                if (a[i] > a[maxIndex])
                    maxIndex = i;
            }
            int[] length = new int[2];
            int index = 0;
            for(int i = 0; i < 4; i++)
            {
                if(i != minIndex && i != maxIndex)
                {
                    length[index] = a[i];
                    index++;
                }
            }
            int result = length[0] - length[1];
            if (result > 0)
                return result;
            else
                return -result;
        }

        //判断两个矩形框是否为同一个对象
        static public bool isTheSame(Rectangle r1, Rectangle r2)
        {
	        if((r1.X+r1.Width)<r2.X || r1.X>(r2.X+r2.Width) || (r1.Y+r1.Height)<r2.Y || r1.Y>(r2.Y+r2.Height))
	        {
        			
	        }else
	        {
		        int [] xLength = {r1.X, r1.X+r1.Width, r2.X, r2.X+r2.Width};
		        int [] yHeight = {r1.Y, r1.Y+r1.Height, r2.Y, r2.Y+r2.Height};
		        double mianji = findLength(xLength) * findLength(yHeight);
		        double mianji1 = r1.Width*r1.Height;
		        double mianji2 = r2.Width*r2.Height;
		        if((mianji/mianji1 + mianji/mianji2)/2 > 0.65)
		        {
			        return true;
		        }
	        }
	        return false;
        }

        //插入一个新事件
        static public EventNode insertEventNode(ref List<EventNode> eventList, Rectangle r, int frameNum)
        {
            EventNode insert = new EventNode();
            insert.startFrame = frameNum;
            insert.rect = r;
            insert.endFrame = -1;
            insert.mark = true;
            insert.capture = new IntPtr(0);

            //事件跟踪的表头
            insert.trackList = new List<Rectangle>();
            insert.trackList.Add(r);
            eventList.Add(insert);
            return insert;
        }

        //删除整个事件列表
        static public void deleteEventList(ref List<EventNode> eventList)
        {
            eventList.Clear();
        }

        //遍历链表,更新跟踪标记
        static public void bianliEventList(ref List<EventNode> eventList, int endFrame)
        {
            if (eventList.Count == 0)
                return;
            for (int i = 0; i < eventList.Count; i++)
            {
                if (eventList[i].mark == false)
                {
                    if (eventList[i].endFrame == -1)
                        eventList[i].endFrame = endFrame;
                }
                else
                {
                    eventList[i].mark = false;
                }
            }
        }

        //过滤列表中短的事件
        static public void eventFilter(ref List<EventNode> eventList)
        {
            for (int i = 0; i < eventList.Count; i++)
            {
                if (eventList[i].endFrame - eventList[i].startFrame < 39)
                {
                    eventList.RemoveAt(i);
                    i--;
                    continue;
                }
  
            }
        }

        //搜索链表,检查有没有相同事件
        static public EventNode searchEventList(ref List<EventNode> eventList, Rectangle r2)
        {
            for (int i = 0; i < eventList.Count; i++)
            {
                if (eventList[i].endFrame != -1)
                    continue;
                Rectangle r1 = eventList[i].rect;
                if (isTheSame(r1, r2))
                {
                    eventList[i].mark = true;
                    eventList[i].rect = r2;

                    //在跟踪列表插入最后一帧
                    eventList[i].trackList.Add(r2);

                    return eventList[i];
                }
            }
            return null;
        }
    }
}

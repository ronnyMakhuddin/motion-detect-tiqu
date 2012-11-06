#pragma once
#include <cv.h>
#include <highgui.h>
#include <vector>
#include "EventNode.h"
using namespace cv;
class EventNodeOperation
{
public:
	static int findLength(int a[]);       //判断是否同一个对象的辅助函数
	static bool isTheSame(Rect r1, Rect r2);  //判断2个矩形是否为同一对象
	static bool isTheSameDirect(Point lineP1, Point lineP2, Point eventP1, Point eventP2);  //判断两条线的方向是否一致
	static bool isEnterRect(Rect r1, Rect r2);          //判断是否入侵
	static EventNode insertEventNode(vector<EventNode> &eventList, Rect r, int frameNum); //插入一个新事件
	static void deleteEventList(vector<EventNode> &eventList);  //删除整个事件链表
	static void bianliEventList(vector<EventNode> &eventList, int endFrame);  //遍历链表,更新跟踪标记
	static void eventFilter(vector<EventNode> &eventList, int fps);     //过滤列表中过短事件
	static bool searchEventList(vector<EventNode> &eventList, Rect r2, EventNode&node); //搜索链表,检查有没有相同事件
	static void selectAbstractEvent(vector<EventNode>&eventList, Point lineP1, Point lineP2, Point rectP1, Point rectP2); //筛选摘要事件
	
	EventNodeOperation(void);
	~EventNodeOperation(void);
public:
	static Scalar sampleColor[];  //定义5种不同颜色区分不同事件
};


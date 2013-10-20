#include "EventNodeOperation.h"

Scalar EventNodeOperation::sampleColor[]={Scalar(0, 0, 0), Scalar(255, 0, 0), Scalar(0, 255, 0), Scalar(0, 0, 255), Scalar(255, 255, 0) };

int EventNodeOperation::findLength(int a[])
{
	int minIndex = 0, maxIndex = 0;
	for (int i = 0; i < 4; i++)
	{
		if (a[i] < a[minIndex])
			minIndex = i;
		if (a[i] > a[maxIndex])
			maxIndex = i;
	}
	int length[2];
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

bool EventNodeOperation::isTheSame(Rect r1, Rect r2)
{
	if((r1.x+r1.width)<r2.x || r1.x>(r2.x+r2.width) || (r1.y+r1.height)<r2.y || r1.y>(r2.y+r2.height))
	{

	}else
	{
		int xLength[] = {r1.x, r1.x+r1.width, r2.x, r2.x+r2.width};
		int yHeight[] = {r1.y, r1.y+r1.height, r2.y, r2.y+r2.height};
		double mianji = findLength(xLength) * findLength(yHeight);
		double mianji1 = r1.width*r1.height;
		double mianji2 = r2.width*r2.height;
		if((mianji/mianji1 + mianji/mianji2)/2 > 0.65)
		{
			return true;
		}
	}
	return false;
}

//判断两个矩形框之间的关系：
/*
return 0:两个矩形互不相关
return 1:两个矩形是同一个矩形
return 2:r1包含r2，分裂情况
return 3:r2包含r1，合并情况
*/
int EventNodeOperation::rectRelationship(Rect r1, Rect r2)
{
	if((r1.x+r1.width)<r2.x || r1.x>(r2.x+r2.width) || (r1.y+r1.height)<r2.y || r1.y>(r2.y+r2.height))
	{
		return 0;
	}else
	{
		int xLength[] = {r1.x, r1.x+r1.width, r2.x, r2.x+r2.width};
		int yHeight[] = {r1.y, r1.y+r1.height, r2.y, r2.y+r2.height};
		double mianji = findLength(xLength) * findLength(yHeight);
		double mianji1 = r1.width*r1.height;
		double mianji2 = r2.width*r2.height;
		if(mianji/mianji1 > 0.65 && mianji/mianji2 > 0.65)
			return 1;
		if(mianji/mianji1 <= 0.65 && mianji/mianji2 > 0.65)
			return 2;
		if(mianji/mianji1 > 0.65 && mianji/mianji2 <= 0.65)
			return 3;
	}
	return 0;
}

//复制一个EventNode
EventNode EventNodeOperation::copyEventNode(EventNode node)
{
	EventNode newNode;
	newNode.mark = node.mark;
	newNode.startFrame = node.startFrame;
	newNode.endFrame = node.endFrame;
	/*
	newNode.rect.x = node.rect.x;
	newNode.rect.y = node.rect.y;
	newNode.rect.width = node.rect.width;
	newNode.rect.height = node.rect.height;
	*/
	newNode.rect = node.rect;
	for(int i = 0; i < node.trackList.size(); i++)
	{
		Rect r = node.trackList[i];
		newNode.trackList.push_back(r);
	}
	return newNode;
}

bool EventNodeOperation::isTheSameDirect(Point lineP1, Point lineP2, Point eventP1, Point eventP2)  //判断是否按方向运动
{
	Point a(lineP2.x-lineP1.x, lineP2.y-lineP1.y);
	Point b(eventP2.x-eventP1.x, eventP2.y-eventP1.y);
	float ab = a.x*b.x + a.y*b.y;
	float a_ = sqrt(float(a.x*a.x+a.y*a.y));
	float b_ = sqrt(float(b.x*b.x+b.y*b.y));
	float cos = ab/(a_*b_);
	float angel = acos(cos)*180/3.14159;
	if(angel > 0 && angel < 45)
		return true;
	return false;
}

bool EventNodeOperation::isEnterRect(Rect r1, Rect r2)  //判断是否入侵r1为划定的矩形，r2为事件矩形
{
	if((r1.x+r1.width)<r2.x || r1.x>(r2.x+r2.width) || (r1.y+r1.height)<r2.y || r1.y>(r2.y+r2.height))
	{

	}else
	{
		int xLength[] = {r1.x, r1.x+r1.width, r2.x, r2.x+r2.width};
		int yHeight[] = {r1.y, r1.y+r1.height, r2.y, r2.y+r2.height};
		double mianji = findLength(xLength) * findLength(yHeight);
		double mianji1 = r1.width*r1.height;
		double mianji2 = r2.width*r2.height;
		if((mianji/mianji1 > 0.5) || (mianji/mianji2 > 0.5))
		{
			return true;
		}
	}
	return false;
}

EventNode EventNodeOperation::insertEventNode(vector<EventNode> &eventList, Rect r, int frameNum)
{
	EventNode insert;
	insert.startFrame = frameNum;
	insert.rect = r;
	insert.endFrame = -1;
	insert.mark = true;
	//insert.capture = 0;

	//事件跟踪的表头
	insert.trackList.push_back(r);
	eventList.push_back(insert);
	return insert;
}

void EventNodeOperation::deleteEventList(vector<EventNode> &eventList)
{
	eventList.clear();
}

void EventNodeOperation::bianliEventList(vector<EventNode> &eventList, int endFrame)
{
	if (eventList.size() == 0)
		return;
	for (int i = 0; i < eventList.size(); i++)
	{
		if (eventList[i].mark == false)
		{
			if (eventList[i].endFrame == -1)
			{
				eventList[i].endFrame = endFrame;
			}
		}
		else
		{
			eventList[i].mark = false;
		}
	}
}

int EventNodeOperation::bianliEventList(vector<EventNode> &eventList, int endFrame, int fps)
{
	int count = 0;
	if (eventList.size() == 0)
		return count;
	for(vector<EventNode>::iterator iter=eventList.begin(); iter!=eventList.end(); )
	{

		if ((*iter).mark == false)
		{
			count++;
			if ((*iter).endFrame == -1)
			{
				(*iter).endFrame = endFrame;
				if( ((*iter).endFrame - (*iter).startFrame < fps*2) || ((*iter).startFrame < 17))  //2秒存在就记为事件,另外这里要开始帧大于17是因为如果小于17的话opencv无法定位，所以过滤掉
				{
					iter = eventList.erase(iter);
					count--;
					continue;
				}
			}
		}else
		{
			(*iter).mark = false;
		}
		iter++;
	}
	return count;
}

void EventNodeOperation::eventFilter(vector<EventNode> &eventList, int fps)
{
	for(vector<EventNode>::iterator iter=eventList.begin(); iter!=eventList.end(); )
	{
		
		if( ((*iter).endFrame - (*iter).startFrame < fps*2) || ((*iter).startFrame < 17))  //2秒存在就记为事件,另外这里要开始帧大于17是因为如果小于17的话opencv无法定位，所以过滤掉
			iter = eventList.erase(iter);
		else
			iter++ ;
	}
}

void EventNodeOperation::selectAbstractEvent(vector<EventNode>&eventList, Point lineP1, Point lineP2, Point rectP1, Point rectP2)
{
	for(vector<EventNode>::iterator iter=eventList.begin(); iter!=eventList.end(); )
	{
		EventNode node = *iter;
		bool isDirect = true;
		bool isEnter = true;
		if(lineP1.x!=-1)
		{
			Point eventP1(node.trackList[0].x + node.trackList[0].width/2, node.trackList[0].y + node.trackList[0].height/2);
			int endIndex = node.trackList.size()-1;
			Point eventP2(node.trackList[endIndex].x + node.trackList[endIndex].width/2, node.trackList[endIndex].y + node.trackList[endIndex].height/2);
			isDirect = isTheSameDirect(lineP1, lineP2, eventP1, eventP2);
		}
		if(rectP1.x!=-1)
		{
			isEnter = false;
			Rect r1;
			r1.x = rectP1.x>rectP2.x?rectP2.x:rectP1.x; 
			r1.y = rectP1.y>rectP2.y?rectP2.y:rectP1.y;
			r1.width = abs(rectP1.x-rectP2.x);
			r1.height = abs(rectP1.y-rectP2.y);
			for(int i = 0; i < node.trackList.size(); i++)
			{
				Rect r2 = node.trackList[i];
				isEnter = isEnterRect(r1, r2);
				if(isEnter)
					break;
			}
			//
		}

		if(isEnter && isDirect)  //条件不符合就删除,否则指向下一条
			iter++;
		else
			iter = eventList.erase(iter);
	}
}

bool EventNodeOperation::searchEventList(vector<EventNode> &eventList, Rect r2, EventNode&node)
{
	/*
	for (int i = 0; i < eventList.size(); i++)
	{
		if (eventList[i].endFrame != -1)
			continue;
		Rect r1 = eventList[i].rect;
		if (isTheSame(r1, r2))
		{
			eventList[i].mark = true;
			eventList[i].rect = r2;
			//在跟踪列表插入最后一帧
			eventList[i].trackList.push_back(r2);
			node = eventList[i];
			return true;
		}
	}
	return false;
	*/
	int size = eventList.size();
	for (int i = 0; i < size; i++)
	{
		if (eventList[i].endFrame != -1)
			continue;
		Rect r1 = eventList[i].rect;
		int relationship = rectRelationship(r1, r2);
		//return 0:两个矩形互不相关
		//return 1:两个矩形是同一个矩形
		//return 2:r1包含r2，分裂情况
		//return 3:r2包含r1，合并情况
		if (1==relationship)
		{
			eventList[i].mark = true;
			eventList[i].rect = r2;

			//在跟踪列表插入最后一帧
			eventList[i].trackList.push_back(r2);
			node = eventList[i];
			return true;
		}else if(2==relationship)
		{
			//新建一个节点，跟之前的完全一样
			EventNode cloneNode = copyEventNode(eventList[i]);
			eventList.push_back(cloneNode);

			eventList[i].mark = true;
			eventList[i].rect = r2;
			//在跟踪列表插入最后一帧
			eventList[i].trackList.push_back(r2);
			node = eventList[i];
			return true;
		}
	}
	return false;
	
}

EventNodeOperation::EventNodeOperation(void)
{
}


EventNodeOperation::~EventNodeOperation(void)
{
}

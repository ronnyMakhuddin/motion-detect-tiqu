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
				eventList[i].endFrame = endFrame;
		}
		else
		{
			eventList[i].mark = false;
		}
	}
}

void EventNodeOperation::eventFilter(vector<EventNode> &eventList, int fps)
{
	for(vector<EventNode>::iterator iter=eventList.begin(); iter!=eventList.end(); )
	{
		
		if( (*iter).endFrame - (*iter).startFrame < fps*2 )  //2秒存在就记为事件
			iter = eventList.erase(iter);
		else
			iter++ ;
	}
}

bool EventNodeOperation::searchEventList(vector<EventNode> &eventList, Rect r2, EventNode&node)
{
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
}

EventNodeOperation::EventNodeOperation(void)
{
}


EventNodeOperation::~EventNodeOperation(void)
{
}

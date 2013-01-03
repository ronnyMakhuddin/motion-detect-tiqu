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

bool EventNodeOperation::isTheSameDirect(Point lineP1, Point lineP2, Point eventP1, Point eventP2)  //�ж��Ƿ񰴷����˶�
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

bool EventNodeOperation::isEnterRect(Rect r1, Rect r2)  //�ж��Ƿ�����r1Ϊ�����ľ��Σ�r2Ϊ�¼�����
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

	//�¼����ٵı�ͷ
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
				if( ((*iter).endFrame - (*iter).startFrame < fps*2) || ((*iter).startFrame < 17))  //2����ھͼ�Ϊ�¼�,��������Ҫ��ʼ֡����17����Ϊ���С��17�Ļ�opencv�޷���λ�����Թ��˵�
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
		
		if( ((*iter).endFrame - (*iter).startFrame < fps*2) || ((*iter).startFrame < 17))  //2����ھͼ�Ϊ�¼�,��������Ҫ��ʼ֡����17����Ϊ���С��17�Ļ�opencv�޷���λ�����Թ��˵�
			iter = eventList.erase(iter);
		else
			iter++ ;
	}
}

void EventNodeOperation::selectAbstractEvent(vector<EventNode>&eventList, Point lineP1, Point lineP2, Point rectP1, Point rectP2, int color, QString jihe, int waiguan, int leixing, int fps)
{
	QStringList jiheList = jihe.split("_");
	int minW = jiheList[0].toInt();
	int maxW = jiheList[1].toInt();
	int minH = jiheList[2].toInt();
	int maxH = jiheList[3].toInt();
	for(vector<EventNode>::iterator iter=eventList.begin(); iter!=eventList.end(); )
	{
		EventNode node = *iter;
		bool isDirect = true;
		bool isEnter = true;
		bool isColor = true;
		bool isJihe = true;
		bool isWaiguan = true;
		bool isLeixing = true;
		if(lineP1.x!=-1)//����ɸѡ
		{
			Point eventP1(node.trackList[0].x + node.trackList[0].width/2, node.trackList[0].y + node.trackList[0].height/2);
			int endIndex = node.trackList.size()-1;
			Point eventP2(node.trackList[endIndex].x + node.trackList[endIndex].width/2, node.trackList[endIndex].y + node.trackList[endIndex].height/2);
			isDirect = isTheSameDirect(lineP1, lineP2, eventP1, eventP2);
		}
		if(rectP1.x!=-1)//��������ɸѡ
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
		}
		{//�����������������ɸѡ
			int index = node.trackList.size()/2;
			int w = node.trackList[index].width;
			int h = node.trackList[index].height;
			if(w > minW && w < maxW &&
				h > minH && h < maxH)
				isJihe = true;
			else 
				isJihe = false;
			if(waiguan == -1)
			{
				isWaiguan = true;
			}else if(waiguan == 0 && w > h-h/10)
			{
				isWaiguan = true;
			}else if(waiguan == 1 && h > w-w/10)
			{
				isWaiguan = true;
			}else if(waiguan == 2 && abs(w - h) < w/10)
			{
				isWaiguan = true;
			}else
			{
				isWaiguan = false;
			}
		}

		if(color != -1)
		{//��ɫ����ɸѡ
			int cCount = 0;
			for(int i = 0; i < sizeof(node.histomgram)/sizeof(int); i++)
			{
				if(node.histomgram[color] >= node.histomgram[i])
					cCount++;
			}
			if(cCount < 10)
				isColor = false;
		}

		if(leixing != -1)
		{//����ɸѡ
			if(leixing == 0)//��
			{
				if(node.startFrame/fps >= 36 && node.startFrame/fps <= 38
					&&node.endFrame/fps >= 45 && node.endFrame/fps <= 47)
				{
					isLeixing = false;
				}
			}else if(leixing == 1)
			{
				if(node.startFrame/fps >= 36 && node.startFrame/fps <= 38
					&&node.endFrame/fps >= 45 && node.endFrame/fps <= 47)
				{
					isLeixing = true;
				}else
					isLeixing = false;
			}
		}

		if(isEnter && isDirect && isJihe && isWaiguan && isColor && isLeixing)  //���������Ͼ�ɾ��,����ָ����һ��
			iter++;
		else
			iter = eventList.erase(iter);
	}
}

bool EventNodeOperation::searchEventList(vector<EventNode> &eventList, Rect r2, EventNode&node, int&index)
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

			//�ڸ����б�������һ֡
			eventList[i].trackList.push_back(r2);
			node = eventList[i];
			index = i;

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

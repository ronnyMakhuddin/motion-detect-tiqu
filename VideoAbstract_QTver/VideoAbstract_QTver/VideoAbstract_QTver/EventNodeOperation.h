#pragma once
#include <cv.h>
#include <highgui.h>
#include <vector>
#include "EventNode.h"
using namespace cv;
class EventNodeOperation
{
public:
	static int findLength(int a[]);       //�ж��Ƿ�ͬһ������ĸ�������
	static bool isTheSame(Rect r1, Rect r2);  //�ж�2�������Ƿ�Ϊͬһ����
	static EventNode insertEventNode(vector<EventNode> &eventList, Rect r, int frameNum); //����һ�����¼�
	static void deleteEventList(vector<EventNode> &eventList);  //ɾ�������¼�����
	static void bianliEventList(vector<EventNode> &eventList, int endFrame);  //��������,���¸��ٱ��
	static void eventFilter(vector<EventNode> &eventList, int fps);     //�����б��й����¼�
	static bool searchEventList(vector<EventNode> &eventList, Rect r2, EventNode&node); //��������,�����û����ͬ�¼�
	EventNodeOperation(void);
	~EventNodeOperation(void);
public:
	static Scalar sampleColor[];  //����5�ֲ�ͬ��ɫ���ֲ�ͬ�¼�
};


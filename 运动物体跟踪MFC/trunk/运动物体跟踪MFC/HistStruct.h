#pragma once
#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <string>



#include "运动物体跟踪MFC.h"
#include "运动物体跟踪MFCDlg.h"

using namespace std;
class HistStruct
{
public:
	HistStruct(void);
	~HistStruct(void);
};

struct TrackNode
{
	CvRect rect;
	int frameNum;
	TrackNode*next;
};

struct HistNode
{
    CvRect rect;
	CvRect startRect;
	bool mark;
	int num;
	int startFrame;
	int endFrame;
	TrackNode*eventStart;
	TrackNode*eventTempNode;

	CvCapture* capture;
	IplImage* motion;
	IplImage **buf;
	int last;
	IplImage *mhi;

	HistNode*next;
	HistNode*pre;
};

static CvScalar sampleColor[] = {CV_RGB(255,0,0), CV_RGB(0,255,0), CV_RGB(0,0,255), CV_RGB(255,255,0), CV_RGB(0,0,0)};

static int findLength(int a[])
{
	int minIndex=0,maxIndex=0;
	for(int i = 0; i < 4; i++)
	{
		if(a[i]<a[minIndex])
			minIndex = i;
		if(a[i]>a[maxIndex])
			maxIndex = i;
	}
	int length[2], index=0;
	for(int i = 0; i < 4; i++)
	{
		if(i!=minIndex && i!=maxIndex)
		{
			length[index] = a[i];
			index++;
		}
	}
	return abs(length[0]-length[1]);
}

static bool isTheSame(CvRect r1, CvRect r2)
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

//插入节点
static HistNode* insertNode(HistNode*head, CvRect r, int startFrame)
{
	HistNode*node = head;
	while(node->next)
	{
		node=node->next;
	}
	HistNode*insert = (HistNode*)malloc(sizeof(HistNode));/*生成新结点*/ 
	insert->startRect = r;
	insert->rect = r;
	insert->num = node->num + 1 ;
	insert->startFrame = startFrame;
	insert->endFrame = -1;
	node->next = insert;
	insert->next = NULL;
	insert->pre = node;

	//事件的表头
	insert->eventStart = (TrackNode*)malloc(sizeof(TrackNode));
	insert->eventStart->rect = r;
	insert->eventStart->frameNum = startFrame;
	insert->eventStart->next = NULL;

	return insert;
}

//创建链表
static void createHead(HistNode* &head, CvRect r, int startFrame)
{
	head = (HistNode*)malloc(sizeof(HistNode));
	head->rect = r;
	head->startRect = r;
	head->num = 0;
	head->startFrame = startFrame;
	head->endFrame = -1;
	head->pre = NULL;
	head->next = NULL;
	//事件的表头
	head->eventStart = (TrackNode*)malloc(sizeof(TrackNode));
	head->eventStart->rect = r;
	head->eventStart->frameNum = startFrame;
	head->eventStart->next = NULL;
}

static void deleteNode(HistNode*node)
{
	node->pre->next = node->next;
	node->next->pre = node->pre;
	delete node;
}

//删除整个链表
static void deleteList(HistNode*&head)
{
	while(head!=NULL)
	{
		HistNode* p = head;
		head = head->next;
		TrackNode* t_h = p->eventStart;
		while(t_h)
		{
			TrackNode* t_p = t_h;
			t_h = t_h->next;
			delete t_p;
			t_p = NULL;
		}
		delete p;
		p = NULL;
	}
}

//遍历链表
static void bianliNode(HistNode*&head, int endFrame)
{
	HistNode*node = head;
	if(head==NULL)
		return;
	do{
		if(node->mark == false)
		{
			if(node->endFrame == -1)
			{
				node->endFrame = endFrame;
			}
		}else
		{
			node->mark = false;
		}
	}while(node=node->next);
}

//过滤链表中短的事件
static void eventHistFilter(HistNode*&head)
{
	HistNode*tempNode = head;
	while(tempNode)
	{
		if(tempNode->endFrame - tempNode->startFrame < 39)
		{
			if(tempNode == head)
			{
				head = head->next;
				head->pre = NULL;
				delete tempNode;
				tempNode = head;

				HistNode* node = tempNode;
				while(node)
				{
					node->num--;
					node = node->next;
				}
			}else if(tempNode->next == NULL)
			{
				tempNode->pre->next = NULL;
				delete tempNode;
				tempNode = NULL;
			}else
			{
				tempNode->pre->next = tempNode->next;
				tempNode->next->pre = tempNode->pre;
				HistNode* node = tempNode->next;
				delete tempNode;
				tempNode = node;
				while(node)
				{
					node->num--;
					node = node->next;
				}
			}
		}else
		{
			tempNode = tempNode->next;
		}
	}
}

//搜索链表
static HistNode* searchHist(HistNode*head, CvRect r2)
{
	HistNode*node = head;
	
	do{
		if(node->endFrame!=-1)
			continue;
		CvRect r1 = node->rect;
		if(isTheSame(r1, r2))
		{
			node->mark=true;
			node->rect=r2;
			
			//遍历此事件的动作链表，插入最后一帧
			TrackNode* trackNode = node->eventStart;
			while(trackNode->next)
			{
				trackNode = trackNode->next;
			}
		    //生成事件暂定结尾节点
			trackNode->next = (TrackNode*)malloc(sizeof(TrackNode));
			trackNode->next->rect = r2;
			trackNode->next->next = NULL;

			return node;
		}
	}while(node=node->next);
	return NULL;
}

//将所有事件以及轨迹输出到文本文件
static void printEvents(HistNode* head, CString fileName, int totalEvent, int jiangeFrame)
{
	fileName.Append(".txt");
	FILE *fp = fopen(fileName,"w");
	CString totalS;
	CString jiangeS;
	CString startFrame;
	CString endFrame;
	CString eventLength;
	CString frameInfo;
	totalS.Format(_T("%d"), totalEvent);
	jiangeS.Format(_T("%d"), jiangeFrame);
	fprintf(fp, totalS);
	fprintf(fp, " ");
	fprintf(fp, jiangeS);
	fprintf(fp, "\n");
	while(head)
	{
		startFrame.Format(_T("%d"), head->startFrame);
		endFrame.Format(_T("%d"), head->endFrame);
		fprintf(fp, startFrame);
		fprintf(fp, " ");
		fprintf(fp, endFrame);
		fprintf(fp, " ");
		TrackNode*trackNode = head->eventStart;
		int eventLengthI = 0;
		while(trackNode)
		{
			eventLengthI++;
			trackNode = trackNode->next;
		}
		eventLength.Format(_T("%d"), eventLengthI);
		fprintf(fp, eventLength);
		fprintf(fp, " ");

		trackNode = head->eventStart;
		while(trackNode)
		{
			frameInfo.Format(_T("%d"), trackNode->rect.x);
			fprintf(fp, frameInfo);
			fprintf(fp, " ");
			frameInfo.Format(_T("%d"), trackNode->rect.y);
			fprintf(fp, frameInfo);
			fprintf(fp, " ");
			frameInfo.Format(_T("%d"), trackNode->rect.width);
			fprintf(fp, frameInfo);
			fprintf(fp, " ");
			frameInfo.Format(_T("%d"), trackNode->rect.height);
			fprintf(fp, frameInfo);
			if(trackNode = trackNode->next)
				fprintf(fp, " ");
		}
		fprintf(fp, "\n");
		head = head->next;
	}

	fclose(fp);
}

//将事件从文本文件读入
static void readEvents(HistNode* &head, FILE*f, int &jiange)
{
	int startFrame, endFrame, totalEvent, eventLength;
	//FILE*f = fopen(fileName,"r");
	fscanf(f,"%d",&totalEvent);
	fscanf(f,"%d",&jiange);
	//a = (int**)malloc(sizeof(int)*totalEvent);

	if(head)
		deleteList(head);
	head = (HistNode*)malloc(sizeof(HistNode));

	HistNode* node = head;
	for(int i = 0; i < totalEvent; i++)
	{
		if((!node->next) && (i!=0))
		{
			node->next = (HistNode*)malloc(sizeof(HistNode));
			node->next->pre = node;
			node = node->next;
		}
		fscanf(f,"%d",&startFrame);
		fscanf(f,"%d",&endFrame);
		fscanf(f,"%d",&eventLength);
		node->num = i;
		node->startFrame = startFrame;
		node->endFrame = endFrame;
		node->next = NULL;

		int numFrame = endFrame-startFrame;
		//int numPoint = numFrame/jiange;
		int numPoint = eventLength;
		//a[i] = (int*)malloc(sizeof(int)*(4*numPoint));
		int value = 0;
		node->eventStart = (TrackNode*)malloc(sizeof(TrackNode));
		TrackNode*eventTempNode = node->eventStart;
		eventTempNode->next = NULL;
		for(int j = 0; j < numPoint; j++)
		{
			if(j==33)
				int a = 0;
			if((j != 0))
			{
				eventTempNode->next = (TrackNode*)malloc(sizeof(TrackNode));
				eventTempNode = eventTempNode->next;
			}
			fscanf(f,"%d",&value);
			eventTempNode->rect.x = value;
			fscanf(f,"%d",&value);
			eventTempNode->rect.y = value;
			fscanf(f,"%d",&value);
			eventTempNode->rect.width = value;
			fscanf(f,"%d",&value);
			eventTempNode->rect.height = value;
			eventTempNode->next = NULL;
		}
	}
	fclose(f);
}
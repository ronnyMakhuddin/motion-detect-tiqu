#pragma once
#include <stdio.h>
#include <vector>
#include <qfile.h>
#include <QTextCodec>
#include <QTextStream>
#include <qthread.h>
#include "EventNode.h"
#include "VideoAnalyze.h"


class FileOperation
{
public:
	FileOperation(void);
	~FileOperation(void);
public:
	static void writeToFile(char* filePath, int jiange, int fps, int key_jiange, vector<EventNode> eventList);   //��ժҪ��Ϣд���ļ�
	//static void readFromFile(VideoAnalyze a);  //���ļ���ȡժҪ��Ϣ
};


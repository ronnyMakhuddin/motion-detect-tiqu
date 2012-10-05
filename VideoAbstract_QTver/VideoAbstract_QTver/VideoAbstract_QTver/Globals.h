#pragma once
#include <QFileDialog>
#include "VideoAnalyze.h"
class Globals
{
public:
	static QStringList files;
public:
	static int getFileNameFromQString(QString filePath, QString&fileName);  //��ȡ�����ļ���
	static int getFileDirFromQString(QString filePath, QString&fileDir);   //��ȡ�ļ���Ŀ¼��
	static QString getTimeFromFrameNum(int frameNum, int fps);              //��֡�Ż��ʱ��hh:mm:ss
	Globals(void);
	~Globals(void);
public:
	//VideoAnalyze *analyzeThread;
};


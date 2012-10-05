#pragma once
#include <QFileDialog>
#include "VideoAnalyze.h"
class Globals
{
public:
	static QStringList files;
public:
	static int getFileNameFromQString(QString filePath, QString&fileName);  //获取单独文件名
	static int getFileDirFromQString(QString filePath, QString&fileDir);   //获取文件的目录名
	static QString getTimeFromFrameNum(int frameNum, int fps);              //从帧号获得时间hh:mm:ss
	Globals(void);
	~Globals(void);
public:
	//VideoAnalyze *analyzeThread;
};


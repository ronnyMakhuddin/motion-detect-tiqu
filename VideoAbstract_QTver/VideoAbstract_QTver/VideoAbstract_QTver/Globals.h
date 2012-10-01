#pragma once
#include <QFileDialog>
class Globals
{
public:
	static QStringList files;
public:
	static int getFileNameFromQString(QString filePath, QString&fileName);  //获取单独文件名
	static int getFileDirFromQString(QString filePath, QString&fileDir);   //获取文件的目录名
	Globals(void);
	~Globals(void);
};


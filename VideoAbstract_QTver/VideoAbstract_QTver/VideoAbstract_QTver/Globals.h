#pragma once
#include <QFileDialog>
class Globals
{
public:
	static QStringList files;
public:
	static int getFileNameFromQString(QString filePath, QString&fileName);  //��ȡ�����ļ���
	static int getFileDirFromQString(QString filePath, QString&fileDir);   //��ȡ�ļ���Ŀ¼��
	Globals(void);
	~Globals(void);
};


#include "Globals.h"

QStringList Globals::files;

int Globals::getFileNameFromQString(QString filePath, QString&fileName)
{
	QStringList list = filePath.split('\\');
	if(list.count()==0)
		return 1;
	fileName = list[list.count()-1];
	return 0;
}

int Globals::getFileDirFromQString(QString filePath, QString&fileDir)
{
	QStringList list = filePath.split('\\');
	if(list.count()==0)
		return 1;
	for(int i = 0; i < list.count()-1; i++)
	{
		fileDir.append(list[i]);
		fileDir.append('\\');
	}
	return 0;
}

Globals::Globals(void)
{
}

Globals::~Globals(void)
{
}

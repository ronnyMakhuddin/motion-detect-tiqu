#include "Globals.h"

QStringList Globals::files;
int64 Globals::time_debug = 0;

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

QString Globals::getTimeFromFrameNum(int frameNum, int fps)
{
	//QString time;
	int totalSeconds = (int)(frameNum*1.0 / fps);
	int hours = totalSeconds / 3600;
	int minute = (totalSeconds % 3600) / 60;
	int seconds = (totalSeconds % 3600) % 60;
	char timeChar[128];
	sprintf(timeChar, "%d:%d:%d", hours, minute, seconds);
    return QString(timeChar);
}

Globals::Globals(void)
{
}

Globals::~Globals(void)
{
}

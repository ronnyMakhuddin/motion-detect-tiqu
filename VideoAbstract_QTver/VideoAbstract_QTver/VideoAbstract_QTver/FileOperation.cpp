#include "FileOperation.h"


FileOperation::FileOperation(void)
{
}


FileOperation::~FileOperation(void)
{
}

void FileOperation::writeToFile(char* filePath, int jiange, int fps, int key_jiange, vector<EventNode> eventList)
{
	
	FILE*fs = fopen(filePath, "w+");
	fprintf(fs, "%d %d %d %d\n", eventList.size(), jiange, fps, key_jiange);
	for (int i = 0; i < eventList.size(); i++)
	{
		EventNode node = eventList[i];
		int startFrame = node.startFrame;
		int endFrame = node.endFrame;
		fprintf(fs, "%d %d %d ", startFrame, endFrame, node.trackList.size());
		for (int j = 0; j < node.trackList.size(); j++)
		{
			int x = node.trackList[j].x;
			int y = node.trackList[j].y;
			int width = node.trackList[j].width;
			int height = node.trackList[j].height;
			fprintf(fs, "%d %d %d %d ", x, y, width, height);
		}
		fprintf(fs, "\n");
	}
	//fflush(fs);
	fclose(fs);
}

void FileOperation::readFromFile(QString filePath, VideoAnalyze*&thread)
{
	int N = 3;
	//QTextCodec *code = QTextCodec::codecForName("GBK");
	QFile file(filePath);
	file.open(QFile::ReadOnly);
	QTextStream stream(&file);

	QString lineString;
	QStringList data;

	lineString = stream.readLine();
	data = lineString.split(" ");
	int total = data[0].toInt();
	thread->jiange = data[1].toInt();
	thread->fps = data[2].toInt();
	thread->key_jiange = data[3].toInt();
	thread->eventList.clear();

	for (int i = 0; i < total; i++)
	{
		EventNode node;
		QString line = stream.readLine();
		QStringList data = line.split(' ');
		node.startFrame = data[0].toInt();
		node.endFrame = data[1].toInt();
		int trackNum = data[2].toInt();
		for (int j = 0; j < trackNum; j++)
		{
			int x = data[j * 4 + N].toInt();
			int y = data[j * 4 + N + 1].toInt();
			int width = data[j * 4 + N + 2].toInt();
			int height = data[j * 4 + N + 3].toInt();
			Rect r(x, y, width, height);
			node.trackList.push_back(r);
		}
		thread->eventList.push_back(node);
	}
}
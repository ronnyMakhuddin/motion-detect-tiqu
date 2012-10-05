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
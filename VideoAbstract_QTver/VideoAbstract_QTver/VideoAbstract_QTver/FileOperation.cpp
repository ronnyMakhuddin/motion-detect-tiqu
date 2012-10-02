#include "FileOperation.h"


FileOperation::FileOperation(void)
{
}


FileOperation::~FileOperation(void)
{
}

void FileOperation::writeToFile(char* filePath, vector<EventNode> eventList)
{
	/*
	FILE*fs = fopen(filePath, "w+");
	fprintf(fs, 
	sw.Write(Global.eventList.Count.ToString() + " " + Global.jiange.ToString() + " " + Global.fps.ToString() + " " + Global.key_jiange.ToString());
	sw.Write("\r\n");
	for (int i = 0; i < Global.eventList.Count; i++)
	{
		EventNode node = Global.eventList[i];
		int startFrame = node.startFrame;
		int endFrame = node.endFrame;
		sw.Write(startFrame.ToString() + " " + endFrame.ToString() + " " + node.trackList.Count.ToString() + " ");
		for (int j = 0; j < node.trackList.Count; j++)
		{
			int x = node.trackList[j].X;
			int y = node.trackList[j].Y;
			int width = node.trackList[j].Width;
			int height = node.trackList[j].Height;
			sw.Write(x.ToString() + " " + y.ToString() + " " + width.ToString() + " " + height.ToString() + " ");
		}
		sw.Write("\r\n");
	}
	sw.Flush();
	sw.Close();
	fs.Close();
	*/
}
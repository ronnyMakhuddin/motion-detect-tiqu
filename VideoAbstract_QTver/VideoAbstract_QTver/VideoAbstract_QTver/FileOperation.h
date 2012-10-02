#pragma once
#include <stdio.h>
#include <vector>
#include "EventNode.h"
class FileOperation
{
public:
	FileOperation(void);
	~FileOperation(void);
public:
	static void writeToFile(char* filePath, vector<EventNode> eventList);
};


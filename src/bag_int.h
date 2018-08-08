#pragma once

#include <stdlib.h>
#include <vector>
#include <list>
#include "bagNode.h"
#include <mutex>

class bag_int
{
public:
	bag_int();
	~bag_int();
	int size();
	void add(int item);
	int first();
	int next();
	bool isEmpty();
	bool hasNext();
private:
	mutex p_hasNextMutex;
	mutex p_nextMutex;
	mutex p_addMutex;
	int p_size;
	bagNode * p_current;
	bagNode * p_first;
};


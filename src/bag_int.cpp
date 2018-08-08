
#include "bag_int.h"


bag_int::bag_int()
{
	p_first = nullptr;
	p_current = nullptr;
	p_size = 0;
}


bag_int::~bag_int()
{

}

int bag_int::size()
{
	return p_size;
}

void bag_int::add(int item)
{
	lock_guard<mutex> guard(p_addMutex);
	bagNode * oldFirst = new bagNode();
	oldFirst = p_first;
	p_first = new bagNode();
	p_first->item = item;
	p_first->next = oldFirst;
	p_current = p_first;
	p_size++;
}
int bag_int::first()
{
	return p_first->item;
}
int bag_int::next()
{

	if (hasNext())
	{
		int returnVector = p_current->item;
		p_current = p_current->next;
		return returnVector;
	}
	else return NULL;
}

bool bag_int::isEmpty()
{
	return (p_first == nullptr);
}

bool bag_int::hasNext()
{
	//lock_guard<mutex> guard (p_hasNextMutex);
	return (p_current != nullptr);
}



#include "bagNode.h"

bagNode::bagNode()
{
	bagNode::next = nullptr;
}


bagNode::~bagNode()
{
	delete(bagNode::next);
	bagNode::next = nullptr;
}



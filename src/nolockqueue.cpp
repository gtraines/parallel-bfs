/*
 * nolockqueue.cpp
 *
 *  Created on: Apr 26, 2014
 *      Author: graham
 */

#include "nolockqueue.h"

no_lock_queue::no_lock_queue(): head(new node) {
	// TODO Auto-generated constructor stub

}

no_lock_queue::~no_lock_queue() {
	// TODO Auto-generated destructor stub
}

void no_lock_queue::push(int new_value)
{
	std::unique_ptr<int> new_data(new int(new_value));
	counted_node_ptr new_next;
	new_next.ptr=new node;
	new_next.external_count = 1;

	for (;;)
	{

	}

}

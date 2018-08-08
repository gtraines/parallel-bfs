/*
 * nolockqueue.h
 *
 *  Created on: Apr 26, 2014
 *      Author: graham
 */

#ifndef NOLOCKQUEUE_H_
#define NOLOCKQUEUE_H_

#include <thread>
#include <atomic>
#include <iostream>
#include <memory>

using namespace std;

class no_lock_queue {
public:
	no_lock_queue();
	void push(int new_value);
	shared_ptr<int> pop();
	virtual ~no_lock_queue();
private:

	struct node;
	struct counted_node_ptr
	{
		int external_count;
		node * ptr;
	};

	struct node_counter
	{
		unsigned internal_count: 30;
		unsigned external_counters: 2;
	};

	struct node {
		atomic<int*> data;
		atomic<node_counter> count;
		counted_node_ptr next;

		node()
		{
			node_counter new_count;
			new_count.internal_count = 0;
			new_count.external_counters = 2;
			count.store(new_count);

			next.ptr=nullptr;
			next.external_count = 0;
		}
	};
	node* pop_head();
	atomic<node*> head;
	atomic<node*> tail;
};

#endif /* NOLOCKQUEUE_H_ */

/*
 * bfs_pthread.h
 *
 *  Created on: Apr 21, 2014
 *      Author: graham
 */

#ifndef BFS_PTHREAD_H_
#define BFS_PTHREAD_H_


#include <stdlib.h>
#include <vector>
#include <list>
#include <string>
#include <stack>
#include "dagraph.h"
#include "bag_int.h"
#include <boost/chrono.hpp>
#include <thread>
#include <queue>
#include <iostream>
#include <limits>
#include <mutex>

using namespace std;

class bfs_pthread
{
public:
	bfs_pthread(daGraph graph, int source, int numThreads);


	~bfs_pthread();
	bool hasPathTo(int v);
	int distTo(int v);
	vector<int> pathTo(int v);

	void getAllPaths();
	void setpMarked();
	void setpEdgeTo();
	void setpDistTo();
	void bfs();
	void bfsParallel();
	void parallelInitializeGraph();
	void exploreAdjVertices();
	void exploreAdjVerticesParallel();
	void setQueueCounter();
	void getAllPathsSplit();
	void pathFindSplit();

	int currentVertex;
	list<int> p_adjVertices;
	queue<int> p_queueCounter;
	int infinity;
	int p_numThreads;
	int p_source;
	daGraph p_graph;
	queue<int> p_queue;
	mutex p_hasNextMutex;
	mutex p_nextMutex;
	mutex p_addMutex;
	mutex p_counterHasNextMutex;
	mutex p_counterNextMutex;
	mutex p_counterPopMutex;
	struct RangePair {
		int begin;
		int end;
	};
	queue<RangePair> p_ranges;

protected:
	vector<bool> p_marked;
	vector<int> p_edgeTo;
	vector<int> p_distTo;
	void pathFind();
	void pathFind2();
};

#endif /* BFS_PTHREAD_H_ */

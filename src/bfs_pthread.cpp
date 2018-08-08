/*
 * bfs_pthread.cpp
 *
 *  Created on: Apr 21, 2014
 *      Author: graham
 */
// POSIX breadth-first search

#include "bfs_pthread.h"


bfs_pthread::bfs_pthread(daGraph graph, int source, int numThreads)
	: p_graph(graph), p_source(source), p_numThreads(numThreads), infinity(INT_MAX){}


void bfs_pthread::parallelInitializeGraph()
{
	//move each assignment procedure to its own thread
	thread t1(&bfs_pthread::setpMarked, this);
	thread t2(&bfs_pthread::setpDistTo, this);
	thread t3(&bfs_pthread::setpEdgeTo, this);
	thread t4(&bfs_pthread::setQueueCounter, this);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	//cout << endl << "Waiting for bag to finish" << endl;


	return;
}

void bfs_pthread::setpMarked()
{
	for (int i = 0; i < p_graph.size(); i++)
	{
		p_marked.push_back(false);
	}
}

void bfs_pthread::setpDistTo()
{
	for (int i = 0; i < p_graph.size(); i++)
	{
		p_distTo.push_back(infinity);
	}
}

void bfs_pthread::setpEdgeTo()
{
	for (int i = 0; i < p_graph.size(); i++)
	{
		p_edgeTo.push_back(0);
	}
}

void bfs_pthread::setQueueCounter()
{
	for (int i = 0; i < p_graph.size(); i++)
	{
		p_queueCounter.push(i + 1);
	}
}

bfs_pthread::~bfs_pthread()
{
}



void bfs_pthread::exploreAdjVertices()
{
	lock_guard<mutex> guardHasNext(p_hasNextMutex);
	while(!p_adjVertices.empty())
	{
		lock_guard<mutex> guardNext(p_nextMutex);
		int w = p_adjVertices.front();
		p_adjVertices.pop_front();
		//cout << "Exploring vertex " << w << endl;
		if (!p_marked[w])
		{
			p_edgeTo[w] = currentVertex;
			p_distTo[w] = p_distTo[currentVertex] + 1;
			p_marked[w] = true;
			//mutex
			lock_guard<mutex> guardAdd(p_addMutex);
			p_queue.push(w);
		}
		else
		{
			//cout << "All done" << endl;

		}
	}
	this_thread::yield();
}

void bfs_pthread::bfs()
{
	parallelInitializeGraph();
	//cout << endl << "Initialized graph" << endl;

	p_distTo[p_source] = 0;
	p_marked[p_source] = true;

	//make this bag a thread-safe queue
	p_queue.push(p_source);


	vector<std::thread> * threadPool = new vector<std::thread>();
	//cout << "Beginning enumeration" << endl;
	while (!p_queue.empty())
	{

		currentVertex = p_queue.front();
		p_queue.pop();
		//cout << "Current vertex : " << currentVertex << endl;

		// parallelize here
		p_adjVertices = p_graph[currentVertex];

		//cout <<endl << "Kicking off threads " << endl;
		for (int i = 0; i < p_numThreads; i++)
		{
			threadPool->push_back(std::thread(&bfs_pthread::exploreAdjVertices, this));
		}

		for (int i = 0; i < p_numThreads; i++)
		{
			threadPool->back().join();
			threadPool->pop_back();
		}

	}
}

void bfs_pthread::bfsParallel()
{
	parallelInitializeGraph();
	//cout << endl << "Initialized graph" << endl;

	p_distTo[p_source] = 0;
	p_marked[p_source] = true;

	//make this bag a thread-safe queue
	p_queue.push(p_source);


	vector<std::thread> * threadPool = new vector<std::thread>();

	//Run search in single thread until the queue reaches adequate size (number of threads)
	while (p_queue.size() <= p_numThreads)
	{
		if (p_queue.size() == 0)
		{
			cout << endl << "No connections from vertex " << endl;
			return;
		}
		currentVertex = p_queue.front();
		p_queue.pop();
		//cout << "Current vertex : " << currentVertex << endl;

		// parallelize here
		p_adjVertices = p_graph[currentVertex];
		while(!p_adjVertices.empty())
		{
			int w = p_adjVertices.front();
			p_adjVertices.pop_front();
			//cout << "Exploring vertex " << w << endl;
			if (!p_marked[w])
			{
				p_edgeTo[w] = currentVertex;
				p_distTo[w] = p_distTo[currentVertex] + 1;
				p_marked[w] = true;
				//mutex
				p_queue.push(w);
			}
			else
			{
				//cout << "All done" << endl;
			}
		}
	}

	// Threads take over search
	//cout <<endl << "Kicking off threads " << endl;
	for (int i = 0; i < p_numThreads; i++)
	{
		threadPool->push_back(std::thread(&bfs_pthread::exploreAdjVerticesParallel, this));
	}

	for (int i = 0; i < p_numThreads; i++)
	{
		threadPool->back().join();
		threadPool->pop_back();
	}
}

void bfs_pthread::exploreAdjVerticesParallel()
{
	lock_guard<mutex> guardHasNext(p_hasNextMutex);
	while (!p_queue.empty())
	{
		lock_guard<mutex> guardNext(p_nextMutex);
		currentVertex = p_queue.front();
		p_queue.pop();
		//cout << "Current vertex : " << currentVertex << endl;

		// parallelize here
		p_adjVertices = p_graph[currentVertex];
		while(!p_adjVertices.empty())
		{
			int w = p_adjVertices.front();
			p_adjVertices.pop_front();
			//cout << "Exploring vertex " << w << endl;
			if (!p_marked[w])
			{
				p_edgeTo[w] = currentVertex;
				p_distTo[w] = p_distTo[currentVertex] + 1;
				p_marked[w] = true;
				//mutex
				lock_guard<mutex> guardAdd(p_addMutex);
				p_queue.push(w);
			}
			else
			{
				//cout << "All done" << endl;
			}
		}
	}
	std::this_thread::yield();

}

bool bfs_pthread::hasPathTo(int v) {
	return p_marked[v];
}

int bfs_pthread::distTo(int v) {
	return p_distTo[v];
}

vector<int> bfs_pthread::pathTo(int v) {
	if (!hasPathTo(v))
	{
		return vector<int>();
	}
	stack<int> path;
	int x;

	for (x = v; p_distTo[x] != 0; x = p_edgeTo[x])
	{
		path.push(x);
	}
	path.push(x);

	vector<int> vectorPath;
	while (!path.empty())
	{
		vectorPath.push_back(path.top());
		path.pop();
	}
	return vectorPath;
}

void bfs_pthread::getAllPathsSplit()
{
	unsigned long const blockSize = p_graph.size()/p_numThreads;
	int blockStart = 1;
	int blockEnd;

	for (int i = 0; i < p_numThreads; i++)
	{
		blockEnd = blockStart + blockSize;
		if (i == p_numThreads - 1 && (blockEnd < p_graph.size() || blockEnd > p_graph.size()))
		{
			blockEnd = p_graph.size();
		}
		RangePair rangePair;
		rangePair.begin = blockStart;
		rangePair.end = blockEnd;
		p_ranges.push(rangePair);
		blockStart = blockEnd;
	}

	vector<thread> threadPool;
	for (int i = 0; i < p_numThreads; i++)
	{
		threadPool.push_back(thread(&bfs_pthread::pathFindSplit, this));
	}
	for (int i = 0; i < p_numThreads; i++)
	{
		threadPool.back().join();
		threadPool.pop_back();
	}
}

void bfs_pthread::pathFindSplit()
{
	lock_guard<mutex> getRange(p_counterNextMutex);
	RangePair rangePair = p_ranges.front();
	lock_guard<mutex> popRange(p_counterPopMutex);
	p_ranges.pop();

	for (int v = rangePair.begin; v < rangePair.end; v++)
	{
		if (hasPathTo(v)) {
			cout << p_source << " " << " - " << v << " (" << distTo(v) << ") : ";

			vector<int> path = pathTo(v);

			for(int i = 0; i < path.size(); i++) {
				int x = path[i];
				if (x == p_source) cout << x;
				else cout << " - " << x;
			}
			cout << endl;
		}

		else {
			cout << p_source << " to: " << v << " not connected" << endl;
		}
	}
}

void bfs_pthread::getAllPaths()
{
	//Parallelize here
	vector<thread> threadPool;
	for (int i = 0; i < p_numThreads; i++)
	{
		threadPool.push_back(thread(&bfs_pthread::pathFind, this));
	}
	for (int i = 0; i < p_numThreads; i++)
	{
		threadPool.back().join();
		threadPool.pop_back();
	}
}

void bfs_pthread::pathFind()
{
	lock_guard<mutex> counterHasNextGuard(p_counterHasNextMutex);
	while(!p_queueCounter.empty()) {
		lock_guard<mutex> counterNextGuard(p_counterNextMutex);
		int v = p_queueCounter.front();
		//lock_guard<mutex> counterPopMutex(p_counterPopMutex);
		p_queueCounter.pop();
		if (hasPathTo(v)) {
			cout << p_source << " " << " - " << v << " (" << distTo(v) << ") : ";

			vector<int> path = pathTo(v);

			for(int i = 0; i < path.size(); i++) {
				int x = path[i];
				if (x == p_source) cout << x;
				else cout << " - " << x;
			}
			cout << endl;
		}

		else {
			cout << p_source << " to: " << v << " not connected" << endl;
		}

	}
	this_thread::yield();
}

void bfs_pthread::pathFind2()
{
	while (!p_graph.empty())
	{
		int v = p_graph.size() -1;
		p_graph.pop_back();
		if (hasPathTo(v)) {
			cout << p_source << " " << " - " << v << " (" << distTo(v) << ") : ";

			vector<int> path = pathTo(v);

			for(int i = 0; i < path.size(); i++) {
				int x = path[i];
				if (x == p_source) cout << x;
				else cout << " - " << x;
			}
			cout << endl;
		}

		else {
			cout << p_source << " to: " << v << " not connected" << endl;
		}
	}
}





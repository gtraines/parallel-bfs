#pragma once
//  Open MP breadth-first search class header

#include <stdlib.h>
#include <vector>
#include <list>
#include <string>
#include <stack>
#include "dagraph.h"
#include <boost/chrono.hpp>

using namespace std;

class bfs_mthread
{
public:
	bfs_mthread(vector<list<int> > graph, int source);
	~bfs_mthread();
	bool hasPathTo(int v);
	int distTo(int v);
	vector<int> pathTo(int v);
	int infinity;
	void getAllPaths(daGraph graph, int source);
protected:
	void bfs(daGraph, int source);

	vector<bool> p_marked;
	vector<int> p_edgeTo;
	vector<int> p_distTo;
};

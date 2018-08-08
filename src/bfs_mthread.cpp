// OpenMP breadth-first search

#include "bfs_mthread.h"
#include <queue>
#include <stack>
#include <iostream>
#include <limits>
#include <omp.h>

bfs_mthread::bfs_mthread(daGraph graph, int source)
{
	infinity = INT_MAX;

	//move each assignment procedure to its own thread

	for (int i = 0; i < graph.size(); i++)
	{
		p_marked.push_back(false);
		p_distTo.push_back(infinity);
		p_edgeTo.push_back(0);
	}

	bfs(graph, source);
}


bfs_mthread::~bfs_mthread()
{
}


void bfs_mthread::bfs(daGraph graph, int source)
{
	queue<int> q;
	p_distTo[source] = 0;
	p_marked[source] = true;
	q.push(source);

	while (!q.empty())
	{
		int v = q.front();
		q.pop();
		// parallelize here

		int numVertices = graph[v].size();
		vector<int> adjVertices(numVertices);
		list<int> listAdjVertices = graph[v];

		for (int i = 0; i < numVertices; i++)
		{			
			adjVertices[i] = listAdjVertices.front();
			listAdjVertices.pop_front();
		}

#pragma omp parallel for
		for (int i = 0; i < numVertices; i++)
		{
			int w = adjVertices[i];
			
			if (!p_marked[w])
			{
				p_edgeTo[w] = v;
				p_distTo[w] = p_distTo[v] + 1;
				p_marked[w] = true;
#pragma omp critical
				{
					q.push(w);
				}
				
			}
		}
	}
}

bool bfs_mthread::hasPathTo(int v) {
	return p_marked[v];
}

int bfs_mthread::distTo(int v) {
	return p_distTo[v];
}

vector<int> bfs_mthread::pathTo(int v) {
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

void bfs_mthread::getAllPaths(daGraph graph, int source)
{
#pragma omp parallel for
	for (int v = 0; v < graph.size(); v++) {

		if (hasPathTo(v)) {
			cout << source << " " << " - " << v << " (" << distTo(v) << ") : ";

			vector<int> path = pathTo(v);

			for(int i = 0; i < path.size(); i++) {
				int x = path[i];
				if (x == source) cout << x;
				else cout << " - " << x;
			}
			cout << endl;
		}

		else {
		cout << source << " to: " << v << " not connected" << endl;
		}

	}
}

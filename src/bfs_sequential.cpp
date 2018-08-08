
#include "bfs_sequential.h"
#include <queue>
#include <stack>
#include <iostream>
#include <limits>

bfs_sequential::bfs_sequential(daGraph graph, int source)
{
	infinity = INT_MAX;
	for (int i = 0; i < graph.size(); i++)
	{
		p_marked.push_back(false);
		p_distTo.push_back(infinity);
		p_edgeTo.push_back(0);
	}

	bfs(graph, source);
}


bfs_sequential::~bfs_sequential()
{
	
}

void bfs_sequential::bfs(daGraph graph, int source)
{
	

	queue<int> q;
	p_distTo[source] = 0;
	p_marked[source] = true;
	q.push(source);

	while (!q.empty())
	{
		int v = q.front();
		q.pop();
		list<int> adjacentV = graph[v];
		for(int i = 0; i < graph[v].size(); i++)
		{
			int w = adjacentV.front();
			adjacentV.pop_front();
			if (!p_marked[w]) 
			{
				p_edgeTo[w] = v;
				p_distTo[w] = p_distTo[v] + 1;
				p_marked[w] = true;
				q.push(w);
			}
		}
	}
}

bool bfs_sequential::hasPathTo(int v) {
	return p_marked[v];
}

int bfs_sequential::distTo(int v) {
	return p_distTo[v];
}

vector<int> bfs_sequential::pathTo(int v) {
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

bool bfs_sequential::check(daGraph graph, int source)
{
	// check that the distance of source = 0
	if (p_distTo[source] != 0) {
		cout << endl << "distance of source " << source << " to itself = " << p_distTo[source] << endl;
		return false;
	}

	// check for each edge v-w dist[w] <= dist[v] + 1
	// provided that v is reachable from s
	for (int v = 0; v < graph.size(); v++)
	{
		list<int> adjacentV = graph[v];
		for(int i = 0; i < graph[v].size(); i++)
		{
			int w = adjacentV.front();
			adjacentV.pop_front();
			if (hasPathTo(v) != hasPathTo(w)) {
				cout << endl << "edge " << v << "-" << w << endl;
				cout << "hasPathTo(" << v << ") = " << hasPathTo(v) << endl;
				cout << "hasPathTo(" << w << ") = " << hasPathTo(w) << endl;
				return false;
			}
			if (hasPathTo(v) && (p_distTo[w] > p_distTo[v] + 1)) {
				cout << endl << "edge " << v << "-" << w << endl;
				cout << "hasPathTo(" << v << ") = " << hasPathTo(v) << endl;
				cout << "hasPathTo(" << w << ") = " << hasPathTo(w) << endl;
				return false;
			}
		}
	}

	// check that v = edgeTo[w] satisfies distTo[w] + distTo[v] + 1
	// provided v is reachable from s
	for (int w = 0; w < graph.size(); w++)
	{
		if (!hasPathTo(w) || w == source) continue;
		int v = p_edgeTo[w];
		if (p_distTo[w] != p_distTo[v] + 1)
		{
			cout << endl << "shortest path edge " << v << "-" << w << endl;
			cout << "hasPathTo(" << v << ") = " << hasPathTo(v) << endl;
			cout << "hasPathTo(" << w << ") = " << hasPathTo(w) << endl;
			return false;
		}
	}

	return true;

}

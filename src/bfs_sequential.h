// breadth-first sequential class header


#include <stdlib.h>
#include <vector>
#include <list>
#include <string>
#include <stack>
#include <boost/chrono.hpp>
#include "dagraph.h"

using namespace std;

class bfs_sequential
{
public:
	bfs_sequential(vector<list<int> > graph, int source);
	~bfs_sequential();
	bool hasPathTo(int v);
	int distTo(int v);
	vector<int> pathTo(int v);
	bool check(daGraph graph, int source);
	int infinity;

protected:
	void bfs(daGraph graph, int source);
	

	vector<bool> p_marked;
	vector<int> p_edgeTo;
	vector<int> p_distTo;
};

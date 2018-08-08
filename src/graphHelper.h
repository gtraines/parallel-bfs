#pragma once

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/chrono.hpp>
#include "dagraph.h"
#include <omp.h>
#include <algorithm>
#include "bfs_pthread.h"
#include "bfs_mthread.h"

using namespace std;
class graphHelper
{
public:
	graphHelper();
	~graphHelper();
	static void saveGraphToGraphViz(daGraph graph);
	static void printEdgesToScreen(daGraph graph);
	static void printGraphStats(vector<string> fileLines);
	static vector<string> parseGraphFileToStrings(string fileName);
	static daGraph parseStringsToGraph(vector<string> fileLines);
	static void runMthreadTest(daGraph graph, int source);
	static void runPthreadTest(daGraph graph, int source, int numThreads);
};


//============================================================================
// Graham Traines
// Semester Project
// Breadth-First Search Concurrent Optimization
//
//============================================================================

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
//#include <boost/graph/vector_as_graph.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "graphHelper.h"
#include "bfs_sequential.h"
#include <boost/chrono.hpp>
#include "dagraph.h"
#include <thread>
using namespace std;



int main(int argc, char *argv[]) {

	bool debug = true;
	if (debug == true)
	{

	}
	if (argc < 3)
	{
		cerr << endl << "Breadth-first Search Performance Tester" << endl
				<< "Use: ./Project filename sourceNodeNumber threadingType numberThreads" << endl
				<< endl

				<< "MultiThreading types:" << endl
				<< endl
				<< "omp \t optional - to use OpenMP threading; " << endl
				<< "number of threads specified by the command " << endl
				<<  "\"set OMP_NUM_THREADS=<number of threads to use>\"" << endl
				<< endl
				<< "posix \t optional - to use POSIX threading; " << endl
				<< "leave numberThreads off for heuristic determination of " << endl
				<< "number of threads to use or use " << endl
				<< "additional integer argument to manually specify" << endl
				<< endl
				<< "example: ./Project graph.txt 1 posix 4" << endl;
		return 1;
	}
	ifstream file(argv[1]);
	string fileName = argv[1];
	bool useMultiThreaded;
	int source = boost::lexical_cast<int>(argv[2]);

	int numThreads;
	string multiThreadingType;
	bool useHardwareConcurrency;
	if (argc > 3)
	{
		useMultiThreaded = true;
		multiThreadingType = argv[3];
		if (argc > 4)
		{
			numThreads = boost::lexical_cast<int>(argv[4]);
			useHardwareConcurrency = false;
		}
		else useHardwareConcurrency = true;

	}
	else useMultiThreaded = false;

	vector<string> fileLines = graphHelper::parseGraphFileToStrings(fileName);

	graphHelper::printGraphStats(fileLines);

	boost::chrono::process_real_cpu_clock::time_point parseStart = boost::chrono::process_real_cpu_clock::now();

	daGraph g = graphHelper::parseStringsToGraph(fileLines);

	boost::chrono::duration<double, boost::milli> parseTime = boost::chrono::process_real_cpu_clock::now() - parseStart;
	cout << "Parsing strings into graph format took " << parseTime.count() << " milliseconds" << endl;

	//graphHelper::printEdgesToScreen(g);

	if (!useMultiThreaded)
	{
		boost::chrono::process_real_cpu_clock::time_point startBfsSeq = boost::chrono::process_real_cpu_clock::now();
		bfs_sequential * seq = new bfs_sequential(g, source);
		boost::chrono::duration<double, boost::milli> bfsSeqTime = boost::chrono::process_real_cpu_clock::now() - startBfsSeq;

		boost::chrono::process_real_cpu_clock::time_point pathStart = boost::chrono::process_real_cpu_clock::now();
		for (int v = 0; v <= g.size(); v++)
		{
			if (seq->hasPathTo(v)) {
				cout << source << " " << " - " << v << " (" << seq->distTo(v) << ") : ";
				vector<int> path = seq->pathTo(v);

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
		boost::chrono::duration<double, boost::milli> pathFindingLength = boost::chrono::process_real_cpu_clock::now() - pathStart;

		cout << "Sequential BFS search took " << bfsSeqTime.count() << " milliseconds " << endl;
		cout << "Shortest path finding took " << pathFindingLength.count() << " milliseconds " << endl;
	}

	else
	{
		cout.sync_with_stdio(true);
		if (multiThreadingType == "omp")
		{
			graphHelper::runMthreadTest(g, source);
		}
		else if(multiThreadingType == "posix")
		{
			if (useHardwareConcurrency)
			{
				int hardwareHint = (thread::hardware_concurrency() - 1);
				if (hardwareHint != 0)
				{
					numThreads = hardwareHint;
				}
				else
				{
					numThreads = 4;

				}
			}
			cout << "Using " << numThreads << " threads" << endl;

			graphHelper::runPthreadTest(g, source, numThreads);

		}
	}

	return 0;
}


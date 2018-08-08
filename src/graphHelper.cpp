
#include "graphHelper.h"

graphHelper::graphHelper()
{
}


graphHelper::~graphHelper()
{
}

void graphHelper::saveGraphToGraphViz(vector<list<int>> graph)
{
	int vertexID = 0;
	ofstream graphOut("graphOut.dat");
	graphOut << "digraph G {";
	for (std::list<int> &vertex : graph)
	{
		for (int &edge : vertex)
		{
			graphOut << vertexID << " -> ";
			graphOut << edge << ";" << endl;
		}
		graphOut << endl;
		vertexID++;
	}
	graphOut << "}";

	graphOut.close();
}

void graphHelper::printEdgesToScreen(vector<list<int>> graph)
{
	int vertexID = 0;
	for(std::list<int> &vertex : graph)
	{
		cout << "Vertex: " << vertexID << " Edges: ";
		for (int &edge : vertex)
		{
			cout << edge << " | ";
		}
		cout << endl;
		vertexID++;
	}
}

void graphHelper::printGraphStats(vector<string> fileLines){
	cout << "Number of vertices: " << fileLines[0] << endl;
	cout << "Number of edges: " << fileLines[1] << endl;
	cout << "Total file length: " << fileLines.size() << endl;
}

vector<string> graphHelper::parseGraphFileToStrings(string fileName) {
	ifstream inFile(fileName);
	vector<string> fileLines(1);
	int i = 0;
	while (inFile.good())
	{
		getline(inFile, fileLines[i]);
		i++;
		fileLines.resize(i + 1);
	}
	return fileLines;
}
vector<list<int>> graphHelper::parseStringsToGraph(vector<string> fileLines)
{
	int numberVertices;
	istringstream buffer(fileLines[0]);
	buffer >> numberVertices;

	std::vector< std::list<int> > g(numberVertices);

	#pragma omp parallel for
	for (int lineIterator = 2; lineIterator < fileLines.size(); lineIterator++)
	{
		vector<string> strs;
		boost::trim(fileLines[lineIterator]);
		boost::split(strs, fileLines[lineIterator], boost::is_any_of("\t "));

		if (strs.size() == 2 || strs.size() == 3)
		{
			int vertex = boost::lexical_cast<int>(strs[0]);
			int terminalEdge = strs[1] != "" ? boost::lexical_cast<int>(strs[1]) : boost::lexical_cast<int>(strs[2]);
			
			#pragma omp critical
			{
				g[vertex].push_back(terminalEdge);
			}
		}

	}

	return g;
}

void graphHelper::runMthreadTest(daGraph graph, int source)
{
	boost::chrono::process_real_cpu_clock::time_point startBfsMthread = boost::chrono::process_real_cpu_clock::now();

	bfs_mthread * bfsMthread = new bfs_mthread(graph, source);

	boost::chrono::duration<double, boost::milli> bfsMthreadTime = boost::chrono::process_real_cpu_clock::now() - startBfsMthread;


	boost::chrono::process_real_cpu_clock::time_point getAllPathStart = boost::chrono::process_real_cpu_clock::now();
	bfsMthread->getAllPaths(graph, source);
	boost::chrono::duration<double, boost::milli> pathFindingLength = boost::chrono::process_real_cpu_clock::now() - getAllPathStart;

	cout << "OpenMP BFS search took " << bfsMthreadTime.count() << " milliseconds " << endl;
	cout << "OpenMP shortest path finding took " << pathFindingLength.count() << " milliseconds " << endl;
}

void graphHelper::runPthreadTest(daGraph graph, int source, int numThreads)
{
	boost::chrono::process_real_cpu_clock::time_point startBfsPthread = boost::chrono::process_real_cpu_clock::now();

	cout << endl << "BFS Searching" << endl;
	bfs_pthread * bfsPthread = new bfs_pthread(graph, source, numThreads);
	bfsPthread->bfsParallel();
	boost::chrono::duration<double, boost::milli> bfsPthreadTime = boost::chrono::process_real_cpu_clock::now() - startBfsPthread;

	cout << endl << "Getting shortest paths" << endl;
	boost::chrono::process_real_cpu_clock::time_point getAllPathStart = boost::chrono::process_real_cpu_clock::now();
	bfsPthread->getAllPathsSplit();
	boost::chrono::duration<double, boost::milli> pathFindingLength = boost::chrono::process_real_cpu_clock::now() - getAllPathStart;

	cout << "C++11 Threading Library (POSIX) BFS search took " << bfsPthreadTime.count() << " milliseconds " << endl;
	cout << "C++11 Threading Library (POSIX) shortest path finding took " << pathFindingLength.count() << " milliseconds " << endl;

	unsigned long const blockSize = graph.size()/numThreads;
	cout << "Block size: " << blockSize << endl;

	int blockStart = 1;
	int blockEnd;

	for (int i = 0; i < numThreads; i++)
	{
		cout << "Block start: " << blockStart << endl;

		blockEnd = blockStart + blockSize;
		if (i == numThreads - 1 && (blockEnd < graph.size() || blockEnd > graph.size()))
		{
			blockEnd = graph.size();
		}
		cout<< "Block end: " << blockEnd << endl;
		blockStart = blockEnd;
	}

}


// =================================================================
//
// File: main.cpp
// Author: Erik Cabrera González
// Author: Arturo Cristián Díaz López
// Author: Pablo Javier Arreola Velasco
// Date: 01/12/2022
//
// =================================================================
#include <iostream>
#include <fstream>
#include <string.h>
#include "ugraph.h"

using namespace std;

// Main
int main(int argc, char *argv[]) {
// Variables iniciales
	int size, numQuery, MNP;
	string nodeA, nodeB, query, vertex;
// input/output
	ifstream input(argv[1], ios::in);
	fstream output(argv[2], ios::out);

	input >> size;

	UMatrixGraph<string> graph(size, false);
	for (int i = 0; i < size; i++) {
		input >> nodeA;
		input >> nodeB;
		graph.addEdge(nodeA, nodeB);
	}

	input >> numQuery;

	for (int i = 0; i < numQuery; i++) {
		input >> vertex;
		input >> MNP;
		output << "Case " << i + 1 << ": " << getPorts(vertex, graph, MNP)
			   << " ports not reachable from port " << vertex << " with MNP = "
			   << MNP << "." << endl;
	}

	return 0;
}

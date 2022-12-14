// =========================================================
// File: ugraph.h
// Author: Erik Cabrera
// Date: 10 de noviembre 2022
// =========================================================

#ifndef UGRAPH_H_
#define UGRAPH_H_

#include <sstream>
#include <fstream>
#include <set>
#include <map>
#include <list>
#include <stack>
#include <queue>
#include <vector>
#include <iostream>
#include "exception.h"
#include "header.h"

// Class UnweightedGraph

template <class Vertex>
class UnweightedGraph {
public:
	virtual void addEdge(Vertex, Vertex) = 0;
	virtual bool containsVertex(Vertex) const = 0;
	virtual std::vector<Vertex> getVertexes() const = 0;
	virtual std::set<Vertex> getConnectionFrom(Vertex) const = 0;
	virtual std::string toString() const = 0;
};

// Class UMatrixGraph

template <class Vertex>
class UMatrixGraph : public UnweightedGraph<Vertex> {
private:
	int next, size;
	bool direction;
	std::vector<Vertex> vertexes;
	std::vector<std::vector<bool> > edges;

	int indexOf(Vertex v) const;

public:
	UMatrixGraph(int, bool dir = true);
	void addEdge(Vertex from, Vertex to);
	bool containsVertex(Vertex v) const;
	std::vector<Vertex> getVertexes() const;
	std::set<Vertex> getConnectionFrom(Vertex v) const;
	std::string toString() const;
	void getGraph(std::fstream &);
};

template <class Vertex>
UMatrixGraph<Vertex>::UMatrixGraph(int max, bool dir) {
	size = max;
	if (size == 0) {
        throw RangeError();
	}

	next = 0;
	direction = dir;
	vertexes.resize(size);
	edges.resize(size);
	for (int i = 0; i < size; i++) {
		edges[i].resize(size, false);
		edges[i][i] = true;
	}
}

template <class Vertex>
int UMatrixGraph<Vertex>::indexOf(Vertex v) const {
	for (int i = 0; i < next; i++) {
		if (vertexes[i] == v) {
			return i;
		}
	}
	return -1;
}

template <class Vertex>
void UMatrixGraph<Vertex>::addEdge(Vertex from, Vertex to) {
	int fp = indexOf(from);
	if (fp == -1) {
		if (next == size) {
			throw OutOfMemory();
		}

		vertexes[next++] = from;
		fp = next - 1;
	}

	int tp = indexOf(to);
	if (tp == -1) {
		if (next == size) {
			throw OutOfMemory();
		}

		vertexes[next++] = to;
		tp = next - 1;
	}

	edges[fp][tp] = true;
	if (!direction) {
		edges[tp][fp] = true;
	}
}

template <class Vertex>
bool UMatrixGraph<Vertex>::containsVertex(Vertex v) const {
	return (indexOf(v) != -1);
}

template <class Vertex>
std::vector<Vertex> UMatrixGraph<Vertex>::getVertexes() const {
	std::vector<Vertex> result(vertexes);
	return result;
}

template <class Vertex>
std::set<Vertex> UMatrixGraph<Vertex>::getConnectionFrom(Vertex v) const {
	int i = indexOf(v);
	if (i == -1) {
		throw NoSuchElement();
	}

	std::set<Vertex> result;
	for (int j = 0; j < next; j++) {
		if (i != j && edges[i][j]) {
			result.insert(vertexes[j]);
		}
	}
	return result;
}

template <class Vertex>
std::string UMatrixGraph<Vertex>::toString() const {
	std::stringstream aux;

	for (int i = 0; i < next; i++) {
		aux << vertexes[i] << "\t";
		for (int j = 0; j < next; j++) {
			aux << edges[i][j] << "\t";
		}
		aux << "\n";
	}
	aux << "\n";
	return aux.str();
}

template <class Vertex>
void UMatrixGraph<Vertex>::getGraph(std::fstream &input) {
	Vertex from, to;
	while (input >> from >> to) {
		addEdge(from, to);
	}
}

// Depth First Search
// O(n^2)
template <class Vertex>
std::set<Vertex> dfs(const Vertex& start, const UnweightedGraph<Vertex>* graph) {
	std::set<Vertex> visited;
	std::stack<Vertex> pending;
	typename std::set<Vertex>::iterator itr;

	pending.push(start);
	while (!pending.empty()) {
		Vertex v = pending.top();
		pending.pop();
		if (visited.find(v) == visited.end()) {
			visited.insert(v);
			std::set<Vertex> connected = graph->getConnectionFrom(v);
			for (itr = connected.begin(); itr != connected.end(); itr++) {
				pending.push((*itr));
			}
		}
	}
	return visited;
}

// Breadth First Search
// O(n^2)
template <class Vertex>
std::set<Vertex> bfs(const Vertex& start, const UnweightedGraph<Vertex>* graph) {
	std::set<Vertex> visited;
	std::queue<Vertex> pending;
	typename std::set<Vertex>::iterator itr;

	pending.push(start);
	while (!pending.empty()) {
		Vertex v = pending.front();
		pending.pop();
		if (visited.find(v) == visited.end()) {
			visited.insert(v);
			std::set<Vertex> connected = graph->getConnectionFrom(v);
			for (itr = connected.begin(); itr != connected.end(); itr++) {
				pending.push((*itr));
			}
		}
	}
	return visited;
}

// Max vertexes
// 0(n^2)
template <class Vertex>
int getPorts(const Vertex &start, UnweightedGraph<Vertex> &graph, int MNP) {
	std::set<Vertex> visited;
	typedef std::pair<Vertex, int> VertexPair;
	std::queue<VertexPair> q;

	q.push({start, 0});
	while (q.empty() == false) {
		VertexPair vertexPair = q.front();
		q.pop();
		Vertex v1 = vertexPair.first;
		int mnp = vertexPair.second;
		if (visited.find(v1) == visited.end()) {
			visited.insert(v1);
			if (mnp < MNP) {
				for (Vertex w : graph.getConnectionFrom(v1)) {
					q.push({w, mnp + 1});
				}
			}
		}
	}
	return bfs(start, &graph).size() - visited.size();
}

#endif

#include "lexical/graph/Graph.h"
#include <iostream>
#include <string>

using namespace std;
using namespace ecc;

int main() {
	Graph graph;
	graph.addState();
	graph.addState();
	graph.addState();
	cout << graph << endl;
	return 0;
}

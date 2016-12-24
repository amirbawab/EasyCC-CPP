#ifndef LEXICAL_LEXICAL_H_
#define LEXICAL_LEXICAL_H_

#include <memory>
#include <string>
#include "graph/Graph.h"

namespace ecc {

class Lexical {
public:

	/**
	 * Build a graph based on a JSON file
	 * @param fileName JSON file name
	 * @return pointer to the graph object
	 */
	std::shared_ptr<Graph> buildGraph(std::string fileName);
};

}

#endif

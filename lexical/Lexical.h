#ifndef LEXICAL_LEXICAL_H_
#define LEXICAL_LEXICAL_H_

#include <memory>
#include <string>
#include "graph/Graph.h"

namespace ecc {

class Lexical {
public:
	std::shared_ptr<Graph> buildGraph(std::string fileName);
};

}

#endif

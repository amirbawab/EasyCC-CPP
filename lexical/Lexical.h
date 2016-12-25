#ifndef LEXICAL_LEXICAL_H_
#define LEXICAL_LEXICAL_H_

#include <memory>
#include <string>
#include "graph/Graph.h"

namespace ecc {

	class Lexical {
	public:
		Lexical(std::string stateMachineFileName);
	private:
		std::shared_ptr<Graph> graph;
	};

}

#endif

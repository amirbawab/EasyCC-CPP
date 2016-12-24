#ifndef LEXICAL_GRAPH_GRAPH_H_
#define LEXICAL_GRAPH_GRAPH_H_

#include <vector>
#include <memory>
#include <ostream>
#include "State.h"

namespace ecc {

class Graph {
public:

	/**
	 * Get root state
	 */
	std::shared_ptr<State> getRoot() const { return root; }

	/**
	 * Get state and return its id
	 */
	int addState();

	/**
	 * Add transition between two states
	 */
	void addTransition(int fromState, int toState, std::string label);

	/**
	 *
	 */
	std::shared_ptr<State> getStateById(int id) const {return this->states[id];}
private:
	std::vector<std::shared_ptr<State>> states;
	std::shared_ptr<State> root;
	friend std::ostream& operator<<(std::ostream& os, const Graph &graph) {
	    os << "Graph has " << graph.states.size() << " states";
		for(size_t i=0; i < graph.states.size(); i++){
	    	os << "\n\t" << *graph.states[i];
	    }
	    return os;
	}
};
}

#endif

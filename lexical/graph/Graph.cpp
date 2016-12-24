#include "Graph.h"

namespace ecc {
int Graph::addState() {
	int id = this->states.size();
	std::shared_ptr<State> state = std::make_shared<State>(id);
	this->states.push_back(state);
	return id;
}

void Graph::addTransition(int fromState, int toState, std::string label) {
	this->states[fromState]->addTransition(label, this->states[toState]);
}
}

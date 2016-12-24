#include "State.h"

namespace ecc {
void State::addTransition(std::string label, std::shared_ptr<State> state) {
	this->transitionMap[label] = state;
}

std::shared_ptr<State> State::getStateOn(std::string label) {
	if(this->transitionMap.count(label) == 1) {
		return this->transitionMap[label];
	}
	return nullptr;
}

std::string State::getTypeStr() const {
	switch(this->type) {
	case INITIAL:
		return "INITIAL";
	case NORMAL:
		return "NORMAL";
	case FINAL:
		return "FINAL";
	default:
		return "UNDEFINED";
	}
}
}

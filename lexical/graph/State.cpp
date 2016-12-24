#include "State.h"

namespace ecc {

	// Define constants
	const std::string State::INITIAL = "INITIAL";
	const std::string State::NORMAL = "NORMAL";
	const std::string State::FINAL = "FINAL";

	void State::addTransition(std::string label, std::shared_ptr<State> state) {
		this->transitionMap[label] = state;
	}

	std::shared_ptr<State> State::getStateOn(std::string label) {
		if(this->transitionMap.count(label) == 1) {
			return this->transitionMap[label];
		}
		return nullptr;
	}
}

#ifndef LEXICAL_GRAPH_STATE_H_
#define LEXICAL_GRAPH_STATE_H_

#include <string>
#include <map>
#include <memory>
#include <ostream>

namespace ecc {

class State {
public:

	// State type
	static const std::string INITIAL;
	static const std::string NORMAL;
	static const std::string FINAL;

	State(int id) : type(State::NORMAL), id(id), backtrack(false){}

	/**
	 * Add Transition for the current state
	 * @param label Transition label
	 * @param state Destination state
	 */
	void addTransition(std::string label, std::shared_ptr<State> state);

	/**
	 * Get the state if a `label` is read
	 * @param label Transition label
	 * @return pointer to the state or nullptr if not found
	 */
	std::shared_ptr<State> getStateOn(std::string label);

	/**
	 * Get state id
	 * @return state id
	 */
	int getId() const {return id;}

	/**
	 * Get token name
	 * @return token name
	 */
	std::string getTokenName() const {return tokenName;}

	/**
	 * Set token name
	 * @param token name
	 */
	void setTokenName(std::string tokenName){this->tokenName = tokenName;}

	/**
	 * Set backtrack value
	 * @param backtrack true|false
	 */
	void setBacktrack(bool backtrack) { this->backtrack = backtrack;}

	/**
	 * Check backtrack value for state
	 * @return true|false
	 */
	bool mustBacktrack() const {return this->backtrack;}

	/**
	 * Get state type
	 * @return type as enum of STATE
	 */
	const std::string getType() const {return this->type;}

	/**
	 * Set the type
	 * @param type of type STATE
	 */
	void setType(std::string type) {
		this->type = type;
	}

private:

	// Store in a map all transitions from this state
	std::map<std::string, std::shared_ptr<State>> transitionMap;

	// Set the type of the state
	std::string type;

	// Unique id for each state
	int id;

	// Token name (for final states)
	std::string tokenName;

	// Check if parser must backtrack (for final states)
	bool backtrack;

	/**
	 * To string
	 */
	friend std::ostream& operator<<(std::ostream& os, const State &state) {
	    os << state.getId();
	    os << " (" << state.getType() << ")" ;
	    os << ":";
	    for(auto i=state.transitionMap.begin(); i!=state.transitionMap.end(); i++){
	    	os << " (" << i->first << "," << i->second->id << ")";
	    }
	    return os;
	}
};
}

#endif

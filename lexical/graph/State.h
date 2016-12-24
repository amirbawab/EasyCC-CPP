#ifndef LEXICAL_GRAPH_STATE_H_
#define LEXICAL_GRAPH_STATE_H_

#include <string>
#include <map>
#include <memory>
#include <ostream>

namespace ecc {

enum STATE {
	INITIAL,
	NORMAL,
	FINAL
};

class State {
public:
	State(int id) : id(id), backtrack(false){}
	void addTransition(std::string label, std::shared_ptr<State> state);
	std::shared_ptr<State> getStateOn(std::string label);
	int getId() const {return id;}
	std::string getTokenName() const {return tokenName;}
	void setTokenName(std::string tokenName){this->tokenName = tokenName;}
	void setBacktrack(bool backtrack) { this->backtrack = backtrack;}
	bool mustBacktrack() const {return this->backtrack;}
	STATE getType() const {return this->type;}
	std::string getTypeStr() const;
private:
	std::map<std::string, std::shared_ptr<State>> transitionMap;
	STATE type;
	int id;
	std::string tokenName;
	bool backtrack;
	friend std::ostream& operator<<(std::ostream& os, const State &state) {
	    os << state.getId();
	    os << " (" << state.getTypeStr() << ")" ;
	    os << ":";
	    for(auto i=state.transitionMap.begin(); i!=state.transitionMap.end(); i++){
	    	os << " (" << i->first << "," << i->second->id << ")";
	    }
	    return os;
	}
};
}

#endif

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

		State(int id, std::string type) :
				id(id), type(type), backtrack(false){}
		State(int id, std::string type, std::string tokenName, bool backtrack) :
				type(type), id(id), tokenName(tokenName), backtrack(backtrack){}

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
		 * Check backtrack value for state
		 * @return true|false
		 */
		bool mustBacktrack() const {return this->backtrack;}

		/**
		 * Get state type
		 * @return type as enum of STATE
		 */
		const std::string getType() const {return this->type;}

	private:

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
			return os;
		}
	};
}

#endif

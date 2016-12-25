#ifndef LEXICAL_GRAPH_GRAPH_H_
#define LEXICAL_GRAPH_GRAPH_H_

#include <vector>
#include <map>
#include <memory>
#include <string>
#include <ostream>
#include "State.h"

namespace ecc {

	class Graph {
	public:

		// Transition constants
		static const std::string TRANSITION_EOF;
		static const std::string TRANSITION_LOWER_CASE_LETTER;
		static const std::string TRANSITION_UPPER_CASE_LETTER;
		static const std::string TRANSITION_DIGIT;
		static const std::string TRANSITION_POSITIVE;
		static const std::string TRANSITION_OTHER;

		/**
		 * Build a graph based on a JSON file
		 * @param fileName JSON file name
		 * @return pointer to the graph object
		 */
		static std::shared_ptr<Graph> buildGraph(std::string fileName);

		/**
		 * Get root state
		 */
		std::shared_ptr<State> getRoot() const { return root; }

		/**
		 * Get state by id
		 * @return pointer to the state
		 */
		std::shared_ptr<State> getStateById(int id) const {return this->states[id];}

	private:

		// List of all states
		std::vector<std::shared_ptr<State>> states;

		// Store states adjacency
		std::vector<std::map<std::string, int>> adjacencyList;

		// Starting state of the graph
		std::shared_ptr<State> root;

		/**
		 * To string
		 */
		friend std::ostream& operator<<(std::ostream& os, const Graph &graph) {
			os << "Graph has " << graph.states.size() << " states";
			for(size_t i=0; i < graph.states.size(); i++){
				os << "\n\t" << *graph.states[i] << ":";
				for(auto pair : graph.adjacencyList[i]) {
					os << " (" << pair.first  << ", " << pair.second << ")";
				}
			}
			return os;
		}
};
}

#endif

#include "Graph.h"
#include "../../rapidjson/document.h"
#include "../../rapidjson/writer.h"
#include <fstream>
#include <sstream>

namespace ecc {

	// State machine transitions
	const std::string Graph::TRANSITION_EOF = "EOF";
	const std::string Graph::TRANSITION_LOWER_CASE_LETTER = "LOWER_CASE_LETTER";
	const std::string Graph::TRANSITION_UPPER_CASE_LETTER = "UPPER_CASE_LETTER";
	const std::string Graph::TRANSITION_DIGIT = "DIGIT";
	const std::string Graph::TRANSITION_POSITIVE = "POSITIVE";
	const std::string Graph::TRANSITION_OTHER = "OTHER";

	std::shared_ptr<Graph> Graph::buildGraph(std::string fileName) {

		// State machine JSON format
		const char* STATES = "states";
		const char* TRANSITIONS = "transitions";
		const char* TYPE = "type";
		const char* ID = "id";
		const char* TOKEN = "token";
		const char* BACKTRACK = "backtrack";
		const char* FROM = "from";
		const char* TO = "to";
		const char* CHARS = "chars";

		// Load file into string stream
		std::ifstream file(fileName);
		std::stringstream  buffer;
		buffer << file.rdbuf();

		// Prepare an empty graph
		std::shared_ptr<Graph> graph = std::make_shared<Graph>();

		// Parse json
		rapidjson::Document d;
		d.Parse(buffer.str().c_str());

		// Resize vector of states
		graph->states.resize(d[STATES].Size());
		graph->adjacencyList.resize(d[STATES].Size());

		// Store if graph has an initial state
		bool hasInitialState = false;

		// Loop on all states
		for(auto i=d[STATES].Begin(); i != d[STATES].End(); i++) {

			// Create state
			int stateId = (*i)[ID].GetInt();
			const char* type = (*i)[TYPE].GetString();

			// If id already used before
			if(graph->states[stateId]) {
				throw std::runtime_error("Multiple definition for the state: " + std::to_string(stateId));
			}

			// If id is outside bound
			if(stateId < 0 || stateId >= graph->states.size()) {
				throw std::runtime_error("State id must be consecutive (order does not matter) starting from 0");
			}

			// Check the type
			if(std::strcmp(type, State::INITIAL.c_str()) == 0) {
				graph->states[stateId] = std::make_shared<State>(stateId, State::INITIAL);

				// If more than one initial state was found, throw an exception
				if(hasInitialState) {
					throw std::runtime_error("Graph cannot have more than one initial state.");
				}

				// Mark an initial state
				hasInitialState = true;

			} else if(std::strcmp(type, State::NORMAL.c_str()) == 0) {
				graph->states[stateId] = std::make_shared<State>(stateId, State::NORMAL);
			} else if(std::strcmp(type, State::FINAL.c_str()) == 0) {
				graph->states[stateId] = std::make_shared<State>(
						stateId, State::FINAL, (*i)[TOKEN].GetString(), (*i)[BACKTRACK].GetBool());
			} else {
				throw std::runtime_error(std::string("State type undefined: ") + type);
			}
		}

		// Loop on all transitions
		for(auto i=d[TRANSITIONS].Begin(); i != d[TRANSITIONS].End(); i++) {

			// Read from and to data
			int fromStateId = (*i)[FROM].GetInt();
			int toStateId = (*i)[TO].GetInt();

			// State of type initial cannot have incoming transitions
			if(graph->getStateById(toStateId)->getType() == State::INITIAL && fromStateId != toStateId) {
				throw std::runtime_error("A state of type initial cannot have incoming transitions (except from itself)");
			}

			// State of type final cannot have outgoing transitions
			if(graph->getStateById(fromStateId)->getType() == State::FINAL) {
				throw std::runtime_error("A state of type final cannot have outgoing transitions");
			}

			// Read and create the transition labels
			for(auto &v : (*i)[CHARS].GetArray()) {
				if(graph->adjacencyList[fromStateId].count(v.GetString()) == 1) {
					throw std::runtime_error(
							std::string("Multiple definition for transition of label ") +
							v.GetString() + " from state id: " + std::to_string(fromStateId));
				}
				graph->adjacencyList[fromStateId][v.GetString()] = toStateId;
			}
		}

		// If no initial state was found
		if(!hasInitialState) {
			throw std::runtime_error("One of your states must be of type INITIAL");
		}

		// If initial and normal states don't have a transition to OTHER
		for(size_t i=0; i < graph->states.size(); i++) {
			if(graph->states[i]->getType() == State::INITIAL || graph->states[i]->getType() == State::NORMAL) {
				if(graph->adjacencyList[i].count(TRANSITION_OTHER) == 0) {
					throw std::runtime_error("All initial and normal states must have a transition on OTHER");
				}
			}
		}

		return graph;
	}
}

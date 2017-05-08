#include <easycc/Graph.h>
#include <rapidjson/writer.h>
#include <rapidjson/istreamwrapper.h>
#include <fstream>
#include <sstream>
#include <iostream>

namespace ecc {

    // State machine transitions
    const std::string Graph::TRANSITION_EOF = "EOF";
    const std::string Graph::TRANSITION_LOWER_CASE_LETTER = "LOWER_CASE_LETTER";
    const std::string Graph::TRANSITION_UPPER_CASE_LETTER = "UPPER_CASE_LETTER";
    const std::string Graph::TRANSITION_POSITIVE = "POSITIVE";
    const std::string Graph::TRANSITION_OTHER = "OTHER";
    const std::string Graph::TRANSITION_NEW_LINE = "NEW_LINE";
    const std::string Graph::TRANSITION_RETURN = "RETURN";
    const std::string Graph::TRANSITION_SPACE = "SPACE";
    const std::string Graph::TRANSITION_TAB = "TAB";

    std::shared_ptr<Graph> Graph::buildGraphFromFile(std::string fileName) {
        std::ifstream ifs(fileName);
        rapidjson::IStreamWrapper isw(ifs);
        rapidjson::Document d;
        d.ParseStream(isw);
        return buildGraph(d);
    }

    std::shared_ptr<Graph> Graph::buildGraphFromString(std::string data) {
        rapidjson::Document d;
        d.Parse(data.c_str());
        return buildGraph(d);
    }

    // TODO Check if transition is more than one character
    std::shared_ptr<Graph> Graph::buildGraph(rapidjson::Document &d) {

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

        // Prepare an empty graph
        std::shared_ptr<Graph> graph = std::make_shared<Graph>();

        // Resize vector of states
        graph->m_states.resize(d[STATES].Size());
        graph->m_adjacencyList.resize(d[STATES].Size());

        // Store if graph has an initial state
        bool hasInitialState = false;

        // Loop on all states
        for(auto i=d[STATES].Begin(); i != d[STATES].End(); i++) {

            // Create state
            int stateId = (*i)[ID].GetInt();
            const char* type = (*i)[TYPE].GetString();

            // If id is outside bound
            if(stateId < 0 || stateId >= graph->m_states.size()) {
                throw std::runtime_error("State id must be consecutive (order does not matter) starting from 0");
            }

            // If id already used before
            if(graph->m_states[stateId]) {
                throw std::runtime_error("Multiple definition for the state: " + std::to_string(stateId));
            }

            // Check the type
            if(std::strcmp(type, State::INITIAL.c_str()) == 0) {
                graph->m_states[stateId] = std::make_shared<State>(stateId, State::INITIAL);

                // If more than one initial state was found, throw an exception
                if(hasInitialState) {
                    throw std::runtime_error("Graph cannot have more than one initial state.");
                }

                // Mark an initial state
                hasInitialState = true;

                // Store initial state
                graph->m_initialState = graph->m_states[stateId];

            } else if(std::strcmp(type, State::NORMAL.c_str()) == 0) {
                graph->m_states[stateId] = std::make_shared<State>(stateId, State::NORMAL);
            } else if(std::strcmp(type, State::FINAL.c_str()) == 0) {
                graph->m_states[stateId] = std::make_shared<State>(
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

            // If from state id is outside bound
            if(fromStateId < 0 || fromStateId >= graph->m_states.size()) {
                throw std::runtime_error(std::string("A transition must have a defined source and destination states: "
                                                 "from state ") + std::to_string(fromStateId));
            }

            // If to state id is outside bound
            if(toStateId < 0 || toStateId >= graph->m_states.size()) {
                throw std::runtime_error(std::string("A transition must have a defined source and destination states: "
                                                 "to state ") + std::to_string(toStateId));
            }

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
                if(graph->m_adjacencyList[fromStateId].count(v.GetString()) == 1) {
                    throw std::runtime_error(
                            std::string("Multiple definition for transition of label ") +
                            v.GetString() + " from state id: " + std::to_string(fromStateId));
                }
                graph->m_adjacencyList[fromStateId][v.GetString()] = toStateId;
            }
        }

        // If no initial state was found
        if(!hasInitialState) {
            throw std::runtime_error("One of your states must be of type INITIAL");
        }

        // If initial and normal states don't have a transition to OTHER
        for(size_t i=0; i < graph->m_states.size(); i++) {
            if(graph->m_states[i]->getType() == State::INITIAL || graph->m_states[i]->getType() == State::NORMAL) {
                if(graph->m_adjacencyList[i].count(TRANSITION_OTHER) == 0) {
                    throw std::runtime_error("All initial and normal states must have a transition on OTHER");
                }
            }
        }

        return graph;
    }

    int Graph::getStateOnRead(int stateId, char charRead) {

        // Check if state id is correct
        if(stateId < 0 || stateId >= this->m_states.size()) {
            throw std::runtime_error("State id not found!");
        }

        // Check if state id correspond to a final state
        if(this->getStateById(stateId)->getType() == State::FINAL) {
            throw std::runtime_error("Cannot read a char from a final state");
        }

        // Convert charRead to string
        std::string charReadStr = std::string(1, charRead);

        // Check if char key is defined
        if(this->m_adjacencyList[stateId].count(charReadStr) == 1) {
            return this->m_adjacencyList[stateId][charReadStr];
        }

        // Check if in special transitions
        if(charRead >= 'a' && charRead <= 'z'
           && this->m_adjacencyList[stateId].count(Graph::TRANSITION_LOWER_CASE_LETTER) == 1) {
            return this->m_adjacencyList[stateId][Graph::TRANSITION_LOWER_CASE_LETTER];

        } else if(charRead >= 'A' && charRead <= 'Z'
                && this->m_adjacencyList[stateId].count(Graph::TRANSITION_UPPER_CASE_LETTER) == 1) {
            return this->m_adjacencyList[stateId][Graph::TRANSITION_UPPER_CASE_LETTER];

        } else if(charRead >= '1' && charRead <= '9'
                && this->m_adjacencyList[stateId].count(Graph::TRANSITION_POSITIVE) == 1) {
            return this->m_adjacencyList[stateId][Graph::TRANSITION_POSITIVE];

        } else if(charRead == EOF
                && this->m_adjacencyList[stateId].count(Graph::TRANSITION_EOF) == 1) {
            return this->m_adjacencyList[stateId][Graph::TRANSITION_EOF];

        } else if(charRead == '\n'
                  && m_adjacencyList[stateId].find(Graph::TRANSITION_NEW_LINE) != m_adjacencyList[stateId].end()) {
            return m_adjacencyList[stateId][Graph::TRANSITION_NEW_LINE];

        } else if(charRead == '\r'
                  && m_adjacencyList[stateId].find(Graph::TRANSITION_RETURN) != m_adjacencyList[stateId].end()) {
            return m_adjacencyList[stateId][Graph::TRANSITION_RETURN];

        } else if(charRead == ' '
                  && m_adjacencyList[stateId].find(Graph::TRANSITION_SPACE) != m_adjacencyList[stateId].end()) {
            return m_adjacencyList[stateId][Graph::TRANSITION_SPACE];

        } else if(charRead == '\t'
                  && m_adjacencyList[stateId].find(Graph::TRANSITION_TAB) != m_adjacencyList[stateId].end()) {
            return m_adjacencyList[stateId][Graph::TRANSITION_TAB];
        }
        return this->m_adjacencyList[stateId][Graph::TRANSITION_OTHER];
    }
}

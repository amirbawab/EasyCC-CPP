#include "Lexical.h"
#include "../rapidjson/document.h"
#include "../rapidjson/writer.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace ecc {

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

    std::shared_ptr<Graph> Lexical::buildGraph(std::string fileName) {

        // Load file into string stream
        std::ifstream file(fileName);
        std::stringstream  buffer;
        buffer << file.rdbuf();

        // Prepare an empty graph
        std::shared_ptr<Graph> graph = std::make_shared<Graph>();

        // Parse json
        rapidjson::Document d;
        d.Parse(buffer.str().c_str());

        // TODO create all states first, then loop again to set their information

        // Loop on all states
        for(auto i=d[STATES].Begin(); i != d[STATES].End(); i++) {

            // Create state
            int stateId = graph->addState();
            std::shared_ptr<State> state = graph->getStateById(stateId);
            const char* type = (*i)[TYPE].GetString();

            // Check the type
            if(std::strcmp(type, State::INITIAL.c_str()) == 0) {
                state->setType(State::INITIAL);

                // If more than one initial state was found, throw an exception
                if(graph->hasInitialState()) {
                    throw std::runtime_error("Graph cannot have more than one initial state.");
                }

                // Mark an initial state
                graph->setInitialState(true);

            } else if(std::strcmp(type, State::NORMAL.c_str()) == 0) {
                state->setType(State::NORMAL);
            } else if(std::strcmp(type, State::FINAL.c_str()) == 0) {
                state->setType(State::FINAL);

                // Get the token name and backtrack values
                state->setTokenName((*i)[TOKEN].GetString());
                state->setBacktrack((*i)[BACKTRACK].GetBool());
            } else {
                throw std::runtime_error(std::string("State type undefined: ") + type);
            }
        }
        return graph;
    }
}

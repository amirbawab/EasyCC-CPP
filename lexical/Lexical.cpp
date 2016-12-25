#include "Lexical.h"
#include <fstream>
#include <sstream>

namespace ecc {
    Lexical::Lexical(std::string stateMachineFileName, std::string configFileName) {
        this->graph = Graph::buildGraph(stateMachineFileName);
        this->config = Config::buildConfig(configFileName);
    }

    void Lexical::generateLexicalTokens(
            std::string fileName, std::vector<std::shared_ptr<LexicalToken>> &vector) {

        // Keep track of file information
        int line = 1;
        int column = 1;
        int position = 0;

        // Keep track of the state
        std::shared_ptr<State> state = graph->getInitialState();

        // Keep track of the token value created
        std::stringstream tokenValueStream;

        // Read character by character
        char ch;
        std::fstream fin(fileName, std::fstream::in);
        while (fin >> std::noskipws >> ch) {

            // Backtrack if needed
            bool backtrack = true;

            // Must be updated by the states
            while(backtrack) {

                // Jump to the next state
                state = graph->getStateById(graph->getStateOnRead(state->getId(), ch));

                // If new state is a final state
                if (state->getType() == State::FINAL) {

                    // Update backtrack according to the state data
                    backtrack = state->mustBacktrack();

                    // Retrieve the token name
                    std::string tokenName = state->getTokenName();

                    // If shouldn't baktrack, then the current char is part of the token value
                    if(!backtrack) {
                        tokenValueStream << ch;
                    }

                    // Construct token value
                    std::string tokenValue = tokenValueStream.str();

                    // Check the type of the token name
                    if(config->isErrorToken(tokenName)) {
                        vector.push_back(std::make_shared<LexicalToken>(
                                LexicalToken::Type::ERROR_TOKEN, tokenName,
                                tokenValue, line, column, position));
                    } else if(!config->mustIgnoreToken(tokenName)) {
                        tokenName = config->updateTokenName(tokenName, tokenValue);
                        vector.push_back(std::make_shared<LexicalToken>(
                                LexicalToken::Type::NORMAL_TOKEN, tokenName,
                                tokenValue, line, column, position));
                    }

                    // Reset values
                    tokenValueStream.str(std::string());
                } else {

                    // Non-final states does need to backtrack
                    backtrack = false;

                    // Add current read character as part of the token value
                    if(state->getType() == State::NORMAL) {
                        tokenValueStream << ch;
                    }
                }
            }
        }

        // TODO Check the case of \r - to test
        // Check if the read char is a new line
        if(ch == '\n') {
            column = 1;
            line++;
        } else {
            column++;
        }
        position++;
    }
}

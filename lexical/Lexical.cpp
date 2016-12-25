#include "Lexical.h"
#include <fstream>

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

        // Keep track of previous char in case of backtrack
        char prevChar = '\0';

        // Keep track of the state
        std::shared_ptr<State> state = graph->getInitialState();

        // Read character by character
        char ch;
        std::fstream fin(fileName, std::fstream::in);
        while (fin >> std::noskipws >> ch) {

            prevChar = ch;
        }
    }
}

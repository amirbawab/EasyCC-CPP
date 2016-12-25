#ifndef LEXICAL_LEXICAL_H_
#define LEXICAL_LEXICAL_H_

#include <memory>
#include <string>
#include <vector>
#include "graph/Graph.h"
#include "config/Config.h"
#include "token/LexicalToken.h"

namespace ecc {

    class Lexical {
    public:
        Lexical(std::string stateMachineFileName, std::string configFileName);

        /**
         * Analyze a text file
         * @param fileName Text file to be analyzed
         * @param vector which will contain the tokens
         */
        void generateLexicalTokens(std::string fileName, std::vector<std::shared_ptr<LexicalToken>> &vector);
    private:

        // State machine
        std::shared_ptr<Graph> graph;

        // Tokens configuration
        std::shared_ptr<Config> config;
    };
}

#endif

#ifndef EASYCC_CPP_GRAMMAR_H
#define EASYCC_CPP_GRAMMAR_H

#include <string>
#include <map>
#include <vector>

namespace ecc {
    class Grammar {
    public:
        Grammar(std::string grammarFile);
        static bool isTerminal(std::string token);
        static bool isNonTerminal(std::string token);
    private:
        std::map<std::string,std::vector<std::vector<std::string>>> productions;

        /**
         * Parse one line of the grammar file
         * @param line Line to be parse
         * @param lastNonTerminal Keep track of the last non-terminal defined
         */
        void parseGrammarLine(std::string line, std::string &lastNonTerminal);

        /**
         * Process a definition
         * @param LHS Left-hand side of a production
         * @param RHS Right-hand side of a production
         */
        void processDefinition(std::string &LHS, std::string &RHS, bool completeDefinition);
    };
}


#endif //EASYCC_CPP_GRAMMAR_H

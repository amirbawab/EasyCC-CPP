#ifndef EASYCC_CPP_GRAMMAR_H
#define EASYCC_CPP_GRAMMAR_H

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <set>

namespace ecc {
    class Grammar {
    public:
        static const std::string EPSILON;
        static const std::string END_OF_STACK;
        Grammar(std::string grammarFile);
        static bool isTerminal(std::string token);
        static bool isNonTerminal(std::string token);
        static bool isEpsilon(std::string token);

        /**
         * Get first set of a token
         * @param token
         * @return set if the item passed is a non-terminal. Otherwise return nullptr
         */
        std::shared_ptr<std::set<std::string>> getFirstSet(std::string token);

    private:
        std::string start;
        std::map<std::string, std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::string>>>>> productions;
        std::map<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::set<std::string>>> productionFirstSet;
        std::map<std::string, std::shared_ptr<std::set<std::string>>> firstSet;
        std::map<std::string, std::shared_ptr<std::set<std::string>>> followSet;

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

        /**
         * Compute the first set
         */
        void computeFirstSet();

        /**
         * Compute the follow set
         */
        void computFollowSet();
    };
}


#endif //EASYCC_CPP_GRAMMAR_H

#ifndef EASYCC_CPP_GRAMMAR_H
#define EASYCC_CPP_GRAMMAR_H

#include <string>
#include <memory>
#include <map>
#include <vector>
#include <set>
#include <rapidjson/document.h>

namespace ecc {
    class Grammar {
    public:
        static const std::string EPSILON;
        static const std::string END_OF_STACK;
        static bool isTerminal(const std::string &token);
        static bool isNonTerminal(const std::string &token);
        static bool isEpsilon(const std::string &token);
        static bool isSemanticAction(const std::string &token);

        /**
         * Create a grammar pointer
         * @param grammarFileName file name for the grammar file
         * @return grammar pointer
         */
        static std::shared_ptr<Grammar> buildGrammarFromFile(std::string grammarFileName);

        /**
         * Create a grammar pointer
         * @param data JSON data
         * @return grammar pointer
         */
        static std::shared_ptr<Grammar> buildGrammarFromString(std::string data);

        /**
         * Build first and follow sets, and populate the parsing table
         */
        void process();

        /**
         * Get first set of a token
         * @param token
         * @return set if the item passed is a non-terminal. Otherwise return nullptr
         */
        std::shared_ptr<std::set<std::string>> getFirstSet(std::string token);

        /**
         * Get follow set of a token
         * @param token
         * @return set if the item passed is a non-terminal. Otherwise return nullptr
         */
        std::shared_ptr<std::set<std::string>> getFollowSet(std::string token);

        /**
         * Get production stored in the parsing table
         * This function behaves like a lookup table where the row key is the non terminal
         * and the column key is the input.
         * @param nonTerminal Row key
         * @param input Column key
         * @return If combination of keys was found, then return the production,
         * otherwise return nullptr
         */
        std::shared_ptr<std::vector<std::string>>
                getParseTable(const std::string &nonTerminal, const std::string &input);

        /**
         * Get the map for productions stored in the parsing table under
         * a specific non terminal.
         * @param non terminal
         * @return If non terminal exists as a key, return the map pointer, otherwise
         * return nullptr
         */
        std::shared_ptr<std::map<std::string, std::shared_ptr<std::vector<std::string>>>>
                getParseMap(std::string nonTerminal);

        /**
         * Get the root of any grammar derivation tree
         * @return start token
         */
        std::string getStart() const{ return this->m_start;}

        /**
         * Extract token from a terminal token
         * 'TERMINAL' becomes TERMINAL
         * @param terminal
         * @return extracted token
         */
        static std::string extractTerminal(std::string terminal);
    private:
        std::string m_start;
        std::map<std::string, std::shared_ptr<std::vector<std::shared_ptr<std::vector<std::string>>>>> m_productions;
        std::map<std::shared_ptr<std::vector<std::string>>, std::shared_ptr<std::set<std::string>>> m_productionFirstSet;
        std::map<std::string, std::shared_ptr<std::set<std::string>>> m_firstSet;
        std::map<std::string, std::shared_ptr<std::set<std::string>>> m_followSet;
        std::map<std::string, std::shared_ptr<std::map<std::string,
                std::shared_ptr<std::vector<std::string>>>>> m_parseTableMap;

        /**
         * Compute the first set
         */
        void computeFirstSet();

        /**
         * Compute the follow set
         */
        void computeFollowSet();

        /**
        * Check if the grammar is LL
        * A -> B | C
        * Cond 1: No left recursion
        * Cond 2: First(B) ^ First(C) = {}
        * Cond 3: If First(A) contains "", then First(A) & Follow(A) = {}
        */
        void validate();

        /**
         * Checks if the grammar has left recursion
         * @param token
         * @param visitedNonTerminals
         * @return token if left recursion detected. Otherwise return null
         */
        std::string getLeftRecursion(std::string token, std::set<std::string> visited);

        /**
         * Log first set
         */
        void logFirstSet();

        /**
         * Log follow set
         */
        void logFollowSet();

        /**
         * Check if the first and follow sets have the same keys
         */
        void checkFFKeys();

        /**
         * Build a parsing table using a map
         * TT[NT][Input[0]] = production | not found
         */
        void buildParseTable();

        /**
         * Create a grammar pointer
         * @param document
         * @return grammar pointer
         */
        static std::shared_ptr<Grammar> buildGrammar(rapidjson::Document &d);
    };
}


#endif

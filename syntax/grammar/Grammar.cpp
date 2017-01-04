#include "Grammar.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace ecc {
    Grammar::Grammar(std::string grammarFile) {

        // Load file
        std::ifstream file(grammarFile);

        // Read line by line
        std::string line;
        std::string lastNonTerminal;
        while (std::getline(file, line)) {
            parseGrammarLine(line, lastNonTerminal);
        }
    }

    void Grammar::parseGrammarLine(std::string line, std::string &lastNonTerminal) {
        std::vector<std::string> words;
        boost::split(words, line, boost::is_any_of(":"), boost::token_compress_on);

        // If two, then it's a complete definition
        if(words.size() == 2) {

            // Process definition
            processDefinition(words[0], words[1], true);

            // Update last non-terminal
            lastNonTerminal = words[0];

        } else if(words.size() == 1) {

            // If no last non-terminal defined (wrong format)
            if(lastNonTerminal.empty()) {
                throw std::runtime_error("Wrong grammar format. No left-hand side non-terminal "
                                                 "token defined at line" + line);
            }

            // Process definition
            processDefinition(lastNonTerminal, words[0], false);
        } else {
            throw std::runtime_error("Wrong grammar format at line: " + line);
        }
    }

    void Grammar::processDefinition(std::string &LHS, std::string &RHS, bool completeDefinition) {

        // Clear whitespaces on both sides
        boost::trim(LHS);

        // If non-terminal not defined
        if (LHS.size() == 0) {
            throw std::runtime_error("Wrong grammar format: A line cannot start with a delimiter");
        }

        // Check if non-terminal is composed of upper case alphabets only
        for (size_t i = 0; i < LHS.size(); i++) {
            if ((LHS[i] < 'A' || LHS[i] > 'Z') && LHS[i] != '_') {
                throw std::runtime_error("Non terminals should be composed of upper case letters only.");
            }
        }

        // Check the set of terminals
        std::vector<std::string> productionVector;
        boost::split(productionVector, RHS, boost::is_any_of("|"), boost::token_compress_on);

        // If production found
        if (productionVector.size() > 0) {

            // If not a complete definition remove the first element
            if(!completeDefinition) {
                if(productionVector[0].size() != 0) {
                    throw std::runtime_error("Wrong grammar format. Expecting a delimiter before " + RHS);
                }
                productionVector.erase(productionVector.begin());
            }

            // Resize corresponding vector
            size_t prevSize = productions[LHS].size();
            productions[LHS].resize(prevSize + productionVector.size());

            // Split production by spaces
            for (size_t i = 0; i < productionVector.size(); i++) {
                boost::trim(productionVector[i]);

                if(productionVector[i].size() == 0) {
                    throw std::runtime_error("A production cannot be empty.");
                } else {
                    std::istringstream productionss(productionVector[i]);

                    // Read word by word
                    std::string word;
                    while (productionss >> word) {

                        // Check if a terminal or an epsilon is defined correctly
                        if((Grammar::isTerminal(word) || Grammar::isEpsilon(word)) &&
                           (productions[LHS][i + prevSize].size() != 0 ||
                            productionss.rdbuf()->in_avail() != 0)) {
                            throw std::runtime_error("A production containing a terminal or an epsilon token "
                                                             "cannot be followed or preceded by other tokens.");
                        }
                        productions[LHS][i + prevSize].push_back(word);
                    }
                }
            }
        } else {
            throw std::runtime_error("Wrong grammar format at the definition of: " + LHS);
        }
    }

    bool Grammar::isTerminal(std::string token) {
        return token[0] == '\'' && token[token.size()-1] == '\'';
    }

    bool Grammar::isNonTerminal(std::string token) {
        return !Grammar::isTerminal(token) && !Grammar::isEpsilon(token);
    }

    bool Grammar::isEpsilon(std::string token) {
        return token == "EPSILON";
    }
}
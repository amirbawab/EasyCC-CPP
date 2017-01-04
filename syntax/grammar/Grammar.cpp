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

        // Print
        for(auto entry : productions) {
            for(auto entryProduction : entry.second) {
                std::cout << entry.first << ":";
                for(auto token : entryProduction) {
                    std::cout << " " << token;
                }
                std::cout << std::endl;
            }
        }
    }

    void Grammar::parseGrammarLine(std::string line, std::string &lastNonTerminal) {
        std::vector<std::string> words;
        boost::split(words, line, boost::is_any_of(":"), boost::token_compress_on);

        // If two, then it's a complete definition
        if(words.size() == 2) {

            // Clear whitespaces on both sides
            boost::trim(words[0]);

            // If non-terminal not defined
            if (words[0].size() == 0) {
                throw std::runtime_error("Wrong grammar format: A line cannot start with a delimiter");
            }

            // Check if non-terminal is composed of upper case alphabets only
            for (size_t i = 0; i < words[0].size(); i++) {
                if ((words[0][i] < 'A' || words[0][i] > 'Z') && words[0][i] != '_') {
                    throw std::runtime_error("Non terminals should be composed of upper case letters only.");
                }
            }

            // Check if multiple definition found for the same non-terminal
            if(productions[words[0]].size() != 0) {
                throw std::runtime_error("Multiple definition found for the same non-terminal: " + words[0]);
            }

            // Update last non-terminal
            lastNonTerminal = words[0];

            // Check the set of terminals
            std::vector<std::string> productionVector;
            boost::split(productionVector, words[1], boost::is_any_of("|"), boost::token_compress_on);

            // If production found
            if (productionVector.size() > 0) {

                // Resize corresponding vector
                productions[words[0]].resize(productionVector.size());

                // Split production by spaces
                for (size_t i = 0; i < productionVector.size(); i++) {
                    boost::trim(productionVector[i]);
                    std::istringstream productionss(productionVector[i]);

                    // Read word by word
                    std::string word;
                    while (productionss >> word) {

                        // Check if a terminal is defined correctly
                        if(Grammar::isTerminal(word) &&
                                    (productions[words[0]][i].size() != 0 ||
                                    productionss.rdbuf()->in_avail() != 0)) {
                            throw std::runtime_error("A production containing a terminal token cannot be followed "
                                                             "or preceded by other tokens.");
                        }
                        productions[words[0]][i].push_back(word);
                    }
                }
            } else {
                throw std::runtime_error("Wrong grammar format at line: " + line);
            }

        } else if(words.size() == 1) {

        } else {
            throw std::runtime_error("Wrong grammar format at line: " + line);
        }
    }

    bool Grammar::isTerminal(std::string token) {
        return token[0] == '\'' && token[token.size()-1] == '\'';
    }

    bool Grammar::isNonTerminal(std::string token) {
        return !isTerminal(token);
    }
}

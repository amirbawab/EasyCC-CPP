#include "Grammar.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace ecc {

    const std::string Grammar::EPSILON = "EPSILON";
    const std::string Grammar::END_OF_STACK = "END_OF_STACK";

    Grammar::Grammar(std::string grammarFile) {

        // Load file
        std::ifstream file(grammarFile);

        // Read line by line
        std::string line;
        std::string lastNonTerminal;
        while (std::getline(file, line)) {
            parseGrammarLine(line, lastNonTerminal);
        }

        // Construct the first set
        computeFirstSet();

        // Construct the follow set
        computFollowSet();

        // Validate LL conditions
        validate();

        // Print follow set
        for(auto entry : followSet) {
            std::cout << entry.first << ": ";
            for(auto val : *entry.second) {
                std::cout << val << ", ";
            }
            std::cout << std::endl;
        }
        std::cout << "DONE" << std::endl;
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

            // Update start token
            if(start.empty()) {
                start = lastNonTerminal;
            }

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

            // If production of LHS was not created, create one
            if(productions.count(LHS) == 0) {
                productions[LHS] = std::make_shared<std::vector<std::shared_ptr<std::vector<std::string>>>>();
            }

            // Resize corresponding vector
            size_t prevSize = productions[LHS]->size();
            productions[LHS]->resize(prevSize + productionVector.size());

            // Split production by spaces
            for (size_t i = 0; i < productionVector.size(); i++) {
                boost::trim(productionVector[i]);

                if(productionVector[i].size() == 0) {
                    throw std::runtime_error("A production cannot be empty.");
                } else {
                    std::istringstream productionss(productionVector[i]);

                    // If production not created, create it
                    if(!(*productions[LHS])[i + prevSize]) {
                        (*productions[LHS])[i + prevSize] = std::make_shared<std::vector<std::string>>();
                    }

                    // Read word by word
                    std::string word;
                    while (productionss >> word) {

                        // Check if a terminal or an epsilon is defined correctly
                        if((Grammar::isTerminal(word) || Grammar::isEpsilon(word)) &&
                           ((*productions[LHS])[i + prevSize]->size() != 0 ||
                            productionss.rdbuf()->in_avail() != 0)) {
                            throw std::runtime_error("A production containing a terminal or an epsilon token "
                                                             "cannot be followed or preceded by other tokens.");
                        }
                        (*productions[LHS])[i + prevSize]->push_back(word);
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
        return token == Grammar::EPSILON;
    }

    void Grammar::computeFirstSet() {

        // Compute the first set multiple times
        for(int z=0; z<productions.size(); z++) {

            // Loop on all definitions
            for(auto &definition : productions) {

                // If first set for a LHS is not defined
                if(firstSet.count(definition.first) == 0) {
                    firstSet[definition.first] = std::make_shared<std::set<std::string>>();
                }

                // Loop on all productions of a definition
                for(auto &production : *definition.second) {

                    // If set not created for a production, create it
                    if(productionFirstSet.count(production) == 0) {
                        productionFirstSet[production] = std::make_shared<std::set<std::string>>();
                    }

                    // Loop on all tokens of a production
                    for(auto const &token : *production) {

                        // If terminal
                        if(Grammar::isTerminal(token) || Grammar::isEpsilon(token)) {
                            productionFirstSet[production]->insert(token);
                            firstSet[definition.first]->insert(token);

                        } else if(Grammar::isNonTerminal(token)) {
                            std::shared_ptr<std::set<std::string>> tokenFirstSet = getFirstSet(token);

                            // If set was defined previously
                            if(tokenFirstSet) {
                                // If epsilon not found in the first set of the token or it's the last token
                                if(tokenFirstSet->find(Grammar::EPSILON) == tokenFirstSet->end() || token == production->back()) {
                                    productionFirstSet[production]->insert(tokenFirstSet->begin(), tokenFirstSet->end());
                                    firstSet[definition.first]->insert(tokenFirstSet->begin(), tokenFirstSet->end());
                                    break;
                                } else {
                                    // Add everything except the epsilon
                                    for(auto firstSetToken : *tokenFirstSet) {
                                        if(!Grammar::isEpsilon(firstSetToken)) {
                                            productionFirstSet[production]->insert(firstSetToken);
                                            firstSet[definition.first]->insert(firstSetToken);
                                        }
                                    }
                                }
                            } else {
                                // This is important because the order of non-terminals matter
                                // especially if we don't know if there's epsilon or not
                                // so the next one should not be evaluated until the current is ready
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    std::shared_ptr<std::set<std::string>> Grammar::getFirstSet(std::string token) {
        if(Grammar::isNonTerminal(token) && firstSet.count(token)) {
            return firstSet[token];
        }
        return nullptr;
    }

    void Grammar::computFollowSet() {

        // Add end of stack to the follow set of the start grammar
        followSet[start] = std::make_shared<std::set<std::string>>();
        followSet[start]->insert(Grammar::END_OF_STACK);

        // Compute the follow set multiple times
        for(int z=0; z <productions.size(); z++) {

            // Loop on definitions
            for(auto &definition : productions) {

                // Loop on productions
                for(auto &production : *definition.second) {

                    // Loop on tokens
                    for(int i=0; i < production->size(); i++) {

                        // Store current token
                        std::string current = (*production)[0];

                        // If a non-terminal token, evaluate it
                        if(Grammar::isNonTerminal(current)) {

                            // If set not created, create it
                            if(followSet.count(current) == 0) {
                                followSet[current] = std::make_shared<std::set<std::string>>();
                            }

                            int j = i;
                            while(++j < production->size()) {

                                // Get token
                                std::string token = (*production)[j];

                                // Get first set
                                std::shared_ptr<std::set<std::string>> tokenFirstSet = getFirstSet(token);

                                // If first set defined
                                if(tokenFirstSet) {

                                    // Check if there's an epsilon
                                    bool noEpsilon = true;

                                    // Copy the tokens
                                    for(auto value : *tokenFirstSet) {
                                        noEpsilon &= !Grammar::isEpsilon(value);
                                        followSet[current]->insert(value);
                                    }

                                    // Break on epsilon
                                    if(noEpsilon) {
                                        break;
                                    }
                                }
                            }

                            if(j == production->size()) {
                                if(followSet.count(definition.first) == 1) {
                                    followSet[current]->insert(followSet[definition.first]->begin(),
                                                               followSet[definition.first]->end());
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void Grammar::validate() {

        // Cond 1
        for(auto definition : productions) {
            std::string leftRecusiveToken = getLeftRecursion(definition.first, std::set<std::string>());
            if (!leftRecusiveToken.empty()) {
                throw std::runtime_error("Left hand side: " + leftRecusiveToken + " has a left recursion");
            }
        }

        // Loop on non-terminal
        for(auto definition : productions) {

            // Cond 2
            std::set<std::string> uniqueFirstSetValues;
            for(auto production : *productions[definition.first]) {
                for(auto token : *productionFirstSet[production]) {
                    if(uniqueFirstSetValues.find(token) != uniqueFirstSetValues.end()) {
                        throw std::runtime_error("The first set of the rules of the non-terminal: " +
                                                         definition.first + " intersect at " + token);
                    } else {
                        uniqueFirstSetValues.insert(token);
                    }
                }
            }

            // Cond 3
            if(uniqueFirstSetValues.find(Grammar::EPSILON) != uniqueFirstSetValues.end()) {
                // Get the follow set
                std::shared_ptr<std::set<std::string>> tokenFollowSet = followSet[definition.first];
                for(std::string token : uniqueFirstSetValues) {
                    if(tokenFollowSet->find(token) != tokenFollowSet->end()) {
                        std::stringstream intersection;
                        std::copy(uniqueFirstSetValues.begin(), uniqueFirstSetValues.end(), std::ostream_iterator<std::string>(intersection, " "));
                        throw std::runtime_error("The first and follow sets of the non-terminal: " + definition.first + " intersect at " + intersection.str());
                    }
                }
            }
        }
    }

    std::string Grammar::getLeftRecursion(std::string token, std::set<std::string> visited) {

        // If visited more than once
        if(visited.find(token) != visited.end()) {
            return token;
        }

        // Mark non terminal as visited
        visited.insert(token);

        for(auto production : *productions[token]) {
            for(std::string syntaxToken : *production) {
                if(Grammar::isNonTerminal(syntaxToken)) {

                    // Check recursively for left-recursion
                    std::string result = getLeftRecursion(syntaxToken, visited);
                    if(!result.empty()) {
                        return result;
                    }

                    // Stop checking when the first set of non-terminal does not contain epsilon
                    if(firstSet[syntaxToken]->find(Grammar::EPSILON) == firstSet[syntaxToken]->end()) {
                        break;
                    }
                } else if(Grammar::isTerminal(syntaxToken)) {
                    // Stop checking if token is a terminal
                    break;
                }
            }
        }

        // No left recursion found
        return "";
    }
}

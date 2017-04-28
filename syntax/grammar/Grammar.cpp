#include "Grammar.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
namespace src = boost::log::sources;
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(ecc_logger, src::logger_mt)

namespace ecc {

    const std::string Grammar::EPSILON = "EPSILON";

    // End of stack symbol informs the parser that the grammar cannot be derived further
    // This symbol must match the value of END_OF_FILE in lexical analysis
    const std::string Grammar::END_OF_STACK = "$";

    Grammar::Grammar(std::string grammarFile) {

        // Load file
        std::ifstream file(grammarFile);

        // Read line by line
        std::string line;
        std::string lastNonTerminal;
        while (std::getline(file, line)) {
            parseGrammarLine(line, lastNonTerminal);
        }

        BOOST_LOG(ecc_logger::get()) << "Computing first set ...";

        // Construct the first set
        computeFirstSet();

        // Print the logs for the first set
        logFirstSet();

        BOOST_LOG(ecc_logger::get()) << "Computing follow set ...";

        // Construct the follow set
        computeFollowSet();

        // Print the logs for the follow set
        logFollowSet();

        BOOST_LOG(ecc_logger::get()) << "Checking if the keys size for the first set matches "
                                                "the keys size for the follow set ...";
        if(firstSet.size() != followSet.size()) {
            throw std::runtime_error("Not all non-terminals were assigned a production");
        }

        BOOST_LOG(ecc_logger::get()) << "Checking if the grammar satisfies the LL conditions ...";

        // Validate LL conditions
        validate();

        BOOST_LOG(ecc_logger::get()) << "Building parsing table ...";

        // Build the parsing table
        buildParseTable();

        BOOST_LOG(ecc_logger::get()) << "Grammar parsed successfully!";
    }

    void Grammar::parseGrammarLine(std::string line, std::string &lastNonTerminal) {
        std::vector<std::string> words;
        boost::split(words, line, boost::is_any_of("->"), boost::token_compress_on);

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

    bool isEmptyWithIgnoreExceptions(std::shared_ptr<std::vector<std::string>> production) {
        for(auto &token : *production) {

            // Semantic action is an exception
            if(!Grammar::isSemanticAction(token)){
                return false;
            }
        }
        return true;
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
                boost::trim(productionVector[0]);
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

                        // Terminal and epsilon tokens cannot be mixed with other tokens except the ones
                        // specified in the isEmptyWithIgnoreExceptions() function
                        if((Grammar::isTerminal(word) || Grammar::isEpsilon(word)) &&
                                !isEmptyWithIgnoreExceptions((*productions[LHS])[i+prevSize])) {
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

    void Grammar::logFirstSet() {
        BOOST_LOG(ecc_logger::get()) << "Computed First Set:";
        std::string result;
        for(auto i = firstSet.begin(); i != firstSet.end(); i++) {
            result = i->first + ": ";
            for(auto j : *i->second) {
                result += j + " ";
            }
            BOOST_LOG(ecc_logger::get()) << result;
        }
    }

    void Grammar::logFollowSet() {
        BOOST_LOG(ecc_logger::get()) << "Computed Follow Set:";
        std::string result;
        for(auto i = followSet.begin(); i != followSet.end(); i++) {
             result = i->first + ": ";
            for(auto j : *i->second) {
                result += j + " ";
            }
            BOOST_LOG(ecc_logger::get()) << result;
        }
    }

    bool Grammar::isTerminal(const std::string &token) {
        return token[0] == '\'' && token[token.size()-1] == '\'';
    }

    bool Grammar::isSemanticAction(const std::string &token) {
        return token[0] == '#' && token[token.size()-1] == '#';
    }

    bool Grammar::isNonTerminal(const std::string &token) {
        return !Grammar::isTerminal(token) && !Grammar::isSemanticAction(token) && !Grammar::isEpsilon(token);
    }

    bool Grammar::isEpsilon(const std::string &token) {
        return token == Grammar::EPSILON;
    }

    /**
     * Get the last non-terminal from a production.
     */
    std::string lastNonTerminal(std::shared_ptr<std::vector<std::string>> production) {
        for(auto rit = production->rbegin(); rit != production->rend(); ++rit) {
            if(Grammar::isNonTerminal(*rit)) {
                return *rit;
            }
        }
        throw std::runtime_error("Production does not contain any non-terminal");
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

                        /**
                         * Check the type of the current token, if terminal or epsilon then add them.
                         * If it's a non-terminal then it needs to be processed.
                         */
                        if(Grammar::isTerminal(token)) {
                            std::string extractedTerminal = extractTerminal(token);
                            productionFirstSet[production]->insert(extractedTerminal);
                            firstSet[definition.first]->insert(extractedTerminal);
                        } else if(Grammar::isEpsilon(token)) {
                            productionFirstSet[production]->insert(token);
                            firstSet[definition.first]->insert(token);
                        } else if(Grammar::isNonTerminal(token)) {
                            std::shared_ptr<std::set<std::string>> tokenFirstSet = getFirstSet(token);

                            // If set was defined previously
                            if(tokenFirstSet) {

                                // If epsilon not found in the first set of the current token or
                                // the current token is the last one in the production, then add all
                                // the tokens to the first set and don't continue evaluating the next tokens
                                // Example 1:
                                //      A -> B C D
                                //      B -> 'T1' | 'T2'
                                //      ...
                                // Then First(A) contains First(B)
                                // Example 2:
                                //      A -> B C D
                                //      B -> EPSILON | 'T1'
                                //      C -> 'T2' | 'T3'
                                //      ...
                                // Then First(A) contains First(B)-{EPSILON} U First(C)
                                // Example 3:
                                //      A -> B C D
                                //      B -> EPSILON | 'T1'
                                //      C -> EPSILON | 'T2'
                                //      D -> EPSILON | 'T3'
                                //      ...
                                // Then First(A) contains First(B)-{EPSILON} U First(C)-{EPSILON} U First(D).
                                // Note that in Example 3, EPSILON will be in First(A)
                                if(tokenFirstSet->find(Grammar::EPSILON) == tokenFirstSet->end() ||
                                        token == lastNonTerminal(production)) {
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
        if(Grammar::isNonTerminal(token) && firstSet.find(token) != firstSet.end()) {
            return firstSet[token];
        }
        return nullptr;
    }

    std::shared_ptr<std::set<std::string>> Grammar::getFollowSet(std::string token) {
        if(Grammar::isNonTerminal(token) && followSet.find(token) != followSet.end()) {
            return followSet[token];
        }
        return nullptr;
    }

    void Grammar::computeFollowSet() {

        // Add end of stack to the follow set of the start grammar
        followSet[start] = std::make_shared<std::set<std::string>>();
        followSet[start]->insert(Grammar::END_OF_STACK);

        // Compute the follow set multiple times
        for(int z=0; z <productions.size(); z++) {

            // Loop on definitions
            for(auto definition : productions) {

                // Loop on productions
                for(auto production : *definition.second) {

                    // Loop on tokens
                    for(int i=0; i < production->size(); i++) {

                        // Store current token
                        std::string current = (*production)[i];

                        // If the current token is a non-terminal then iterate on the next token
                        // and add its first set to the follow set of the current token except for
                        // the epsilon value. If the first set had an epsilon value, then the next token
                        // needs to be evaluated as well. This repeats until no epsilon is found or if
                        // there are no more tokens in the production, which in that case the follow set
                        // of the LHS is added to the follow set of the current token.
                        // Example 1
                        //      A -> B C D
                        //      Follow(A) = {'T1'}
                        //      First(C) = {'T2', EPSILON}
                        //      First(D) = {'T3'}
                        // Then Follow(B) contains First(C)-{EPSILON} U First(D)
                        // Example 2
                        //      A -> B C D
                        //      Follow(A) = {'T1'}
                        //      First(C) = {'T2', EPSILON}
                        //      First(D) = {'T3', EPSILON}
                        // Then Follow(B) contains First(C)-{EPSILON} U First(D)-{EPSILON} U Follow(A)
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
                                        if(!Grammar::isEpsilon(value)) {
                                            followSet[current]->insert(value);
                                        } else {
                                            noEpsilon = false;
                                        }
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
            std::string leftRecursiveToken = getLeftRecursion(definition.first, std::set<std::string>());
            if (!leftRecursiveToken.empty()) {
                throw std::runtime_error("Left hand side: " + leftRecursiveToken + " has a left recursion");
            }
        }

        // Cond 2
        for(auto definition : productions) {
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
        }

        // Cond 3
        for(auto definition : productions) {
            if (firstSet[definition.first]->find(Grammar::EPSILON) != firstSet[definition.first]->end()) {
                // Get the follow set
                std::shared_ptr<std::set<std::string>> tokenFollowSet = followSet[definition.first];
                for (std::string token : *firstSet[definition.first]) {
                    if (tokenFollowSet->find(token) != tokenFollowSet->end()) {
                        std::stringstream intersection;
                        std::copy(firstSet[definition.first]->begin(), firstSet[definition.first]->end(),
                                  std::ostream_iterator<std::string>(intersection, " "));
                        throw std::runtime_error("The first and follow sets of the non-terminal: " + definition.first +
                                                 " intersect at " + intersection.str());
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

    void Grammar::buildParseTable() {

        // Loop on all definitions
        for(auto &definition : productions) {

            // Create map
            parseTableMap[definition.first] =
                    std::make_shared<std::map<std::string, std::shared_ptr<std::vector<std::string>>>>();

            // Loop on all productions
            for(auto &production : *definition.second) {

                // Get the first set of the production
                std::shared_ptr<std::set<std::string>> pFirstSet = productionFirstSet[production];

                // The map key will always be unique since the grammar
                // passed the LL condition tests

                // Couple each token in the first set to the production, except for epsilon
                bool hasEpsilon = false;
                for(std::string token : *pFirstSet) {
                    if(!Grammar::isEpsilon(token)) {
                        (*parseTableMap[definition.first])[token] = production;
                    } else {
                        hasEpsilon = true;
                    }
                }

                // Check if first set contains epsilon, then map the follow set
                if(hasEpsilon) {
                    // Get the follow set of the production
                    std::shared_ptr<std::set<std::string>> pFollowSet = followSet[definition.first];

                    // Couple each token in the follow set to the production
                    for(std::string token : *pFollowSet) {
                        (*parseTableMap[definition.first])[token] = production;
                    }
                }
            }
        }
    }

    std::shared_ptr<std::vector<std::string>> Grammar::getParseTabel(
            const std::string &nonTerminal, const std::string &input) {
        if(parseTableMap.find(nonTerminal) != parseTableMap.end() &&
                    (*parseTableMap[nonTerminal]).find(input) != (*parseTableMap[nonTerminal]).end()) {
            return (*parseTableMap[nonTerminal])[input];
        }
        return nullptr;
    }

    std::string Grammar::extractTerminal(std::string terminal) {
        if(terminal.length() > 2 && terminal[0] == '\'' && terminal[terminal.length()-1] == '\'') {
            return terminal.substr(1,terminal.length()-2);
        }
        std::runtime_error("String '" + terminal + "' is not a terminal");
    }
}

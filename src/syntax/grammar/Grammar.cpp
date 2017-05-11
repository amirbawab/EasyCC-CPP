#include <easycc/Grammar.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <rapidjson/istreamwrapper.h>
#include <boost/algorithm/string/join.hpp>

namespace src = boost::log::sources;
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(ecc_logger, src::logger_mt)

namespace ecc {

    const std::string Grammar::EPSILON = "EPSILON";

    // End of stack symbol informs the parser that the grammar cannot be derived further
    // This symbol must match the value of END_OF_FILE in lexical analysis
    const std::string Grammar::END_OF_STACK = "$";


    bool isEmptyWithIgnoreExceptions(std::shared_ptr<std::vector<std::string>> production) {
        for(auto &token : *production) {

            // Semantic action is an exception
            if(!Grammar::isSemanticAction(token)){
                return false;
            }
        }
        return true;
    }

    std::shared_ptr<Grammar> Grammar::buildGrammarFromFile(std::string grammarFileName) {
        std::ifstream ifs(grammarFileName);
        rapidjson::IStreamWrapper isw(ifs);
        rapidjson::Document d;
        d.ParseStream(isw);
        return buildGrammar(d);
    }

    std::shared_ptr<Grammar> Grammar::buildGrammarFromString(std::string data) {
        rapidjson::Document d;
        d.Parse(data.c_str());
        return buildGrammar(d);
    }

    std::shared_ptr<Grammar> Grammar::buildGrammar(rapidjson::Document &d) {

        // Create grammar
        std::shared_ptr<Grammar> grammar = std::make_shared<Grammar>();

        for (auto iter = d.MemberBegin(); iter != d.MemberEnd(); ++iter){
            std::string nonTerminal = iter->name.GetString();
            auto array = iter->value.GetArray();

            // Check if the productions array is empty
            if(array.Size() == 0) {
                throw std::runtime_error("Non terminal " + nonTerminal + " cannot have an empty production array");
            }

            // Check if the key entered is empty
            if(nonTerminal.empty()) {
                throw std::runtime_error("A non-terminal key cannot be empty");
            }

            // Check if non-terminal is composed of upper case alphabets only
            for (size_t i = 0; i < nonTerminal.size(); i++) {
                if ((nonTerminal[i] < 'A' || nonTerminal[i] > 'Z') && nonTerminal[i] != '_') {
                    throw std::runtime_error("Non terminals should be composed of upper case letters only.");
                }
            }

            // Make sure non-terminal key is only defined once
            if(grammar->m_productions.find(nonTerminal) == grammar->m_productions.end()) {
                grammar->m_productions[nonTerminal] =
                        std::make_shared<std::vector<std::shared_ptr<std::vector<std::string>>>>();
            } else {
                throw std::runtime_error("All definition for the non-terminal " + nonTerminal+
                                         " should be grouped in the array value");
            }

            // Store start non-terminal
            if(grammar->m_start.empty()) {
                grammar->m_start = nonTerminal;
            }

            for(auto vIter = array.Begin(); vIter != array.End(); ++vIter) {
                std::string production = vIter->GetString();
                boost::trim(production);

                if(production.size() == 0) {
                    throw std::runtime_error("A production cannot be empty.");
                } else {
                    std::istringstream productionss(production);
                    (*grammar->m_productions[nonTerminal]).push_back(std::make_shared<std::vector<std::string>>());

                    // Read word by word
                    std::string word;
                    while (productionss >> word) {

                        // Terminal and epsilon tokens cannot be mixed with other tokens except the ones
                        // specified in the isEmptyWithIgnoreExceptions() function
                        if((Grammar::isTerminal(word) || Grammar::isEpsilon(word)) &&
                           !isEmptyWithIgnoreExceptions((*grammar->m_productions[nonTerminal]).back())) {
                            throw std::runtime_error("A production containing a terminal or an epsilon token "
                                                             "cannot be followed or preceded by other tokens.");
                        }
                        (*grammar->m_productions[nonTerminal]).back()->push_back(word);
                    }
                }
            }
        }
        return grammar;
    }

    void Grammar::process() {

        // Construct the first set
        computeFirstSet();

        // Print the logs for the first set
        logFirstSet();

        // Construct the follow set
        computeFollowSet();

        // Print the logs for the follow set
        logFollowSet();

        BOOST_LOG(ecc_logger::get()) << "Checking if the keys for the first set matches "
                    "the keys size for the follow set ...";

        checkFFKeys();

        BOOST_LOG(ecc_logger::get()) << "Checking if the grammar satisfies the LL conditions ...";

        // Validate LL conditions
        validate();

        BOOST_LOG(ecc_logger::get()) << "Building parsing table ...";

        // Build the parsing table
        buildParseTable();

        BOOST_LOG(ecc_logger::get()) << "Grammar parsed successfully!";
    }

    void Grammar::checkFFKeys() {
		std::vector<std::string> diffKeys;
		for(auto pair : m_firstSet) {
			if(m_followSet.find(pair.first) == m_followSet.end()) {
				diffKeys.push_back(pair.first);
			}
		}
		for(auto pair : m_followSet) {
			if(m_firstSet.find(pair.first) == m_firstSet.end()) {
				diffKeys.push_back(pair.first);
			}
		}
		if(!diffKeys.empty()) {
			std::string joined = boost::algorithm::join(diffKeys, ", ");
			throw std::runtime_error("The following tokens [" + joined + "] either are defined but never "
					"used in a production, or used in a production but never defined");
		}
	}

    void Grammar::logFirstSet() {
        BOOST_LOG(ecc_logger::get()) << "Finished computing first set:";
        std::string result;
        for(auto i = m_firstSet.begin(); i != m_firstSet.end(); i++) {
            result = i->first + ": ";
            for(auto j : *i->second) {
                result += j + " ";
            }
            BOOST_LOG(ecc_logger::get()) << result;
        }
        BOOST_LOG(ecc_logger::get()) << "----------";
    }

    void Grammar::logFollowSet() {
        BOOST_LOG(ecc_logger::get()) << "Finished computing follow set:";
        std::string result;
        for(auto i = m_followSet.begin(); i != m_followSet.end(); i++) {
             result = i->first + ": ";
            for(auto j : *i->second) {
                result += j + " ";
            }
            BOOST_LOG(ecc_logger::get()) << result;
        }
        BOOST_LOG(ecc_logger::get()) << "----------";
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

        BOOST_LOG(ecc_logger::get()) << "Computing first set ...";

        // Compute the first set multiple times
        for(int z=0; z<m_productions.size(); z++) {

            // Loop on all definitions
            for(auto &definition : m_productions) {

                // If first set for a LHS is not defined
                if(m_firstSet.count(definition.first) == 0) {
                    m_firstSet[definition.first] = std::make_shared<std::set<std::string>>();
                }

                // Loop on all productions of a definition
                for(auto &production : *definition.second) {

                    // If set not created for a production, create it
                    if(m_productionFirstSet.count(production) == 0) {
                        m_productionFirstSet[production] = std::make_shared<std::set<std::string>>();
                    }

                    // Loop on all tokens of a production
                    for(auto const &token : *production) {

                        /**
                         * Check the type of the current token, if terminal or epsilon then add them.
                         * If it's a non-terminal then it needs to be processed.
                         */
                        if(Grammar::isTerminal(token)) {
                            std::string extractedTerminal = extractTerminal(token);
                            m_productionFirstSet[production]->insert(extractedTerminal);
                            m_firstSet[definition.first]->insert(extractedTerminal);
                        } else if(Grammar::isEpsilon(token)) {
                            m_productionFirstSet[production]->insert(token);
                            m_firstSet[definition.first]->insert(token);
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
                                    m_productionFirstSet[production]->insert(tokenFirstSet->begin(), tokenFirstSet->end());
                                    m_firstSet[definition.first]->insert(tokenFirstSet->begin(), tokenFirstSet->end());
                                    break;
                                } else {
                                    // Add everything except the epsilon
                                    for(auto firstSetToken : *tokenFirstSet) {
                                        if(!Grammar::isEpsilon(firstSetToken)) {
                                            m_productionFirstSet[production]->insert(firstSetToken);
                                            m_firstSet[definition.first]->insert(firstSetToken);
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
        if(Grammar::isNonTerminal(token) && m_firstSet.find(token) != m_firstSet.end()) {
            return m_firstSet[token];
        }
        return nullptr;
    }

    std::shared_ptr<std::set<std::string>> Grammar::getFollowSet(std::string token) {
        if(Grammar::isNonTerminal(token) && m_followSet.find(token) != m_followSet.end()) {
            return m_followSet[token];
        }
        return nullptr;
    }

    void Grammar::computeFollowSet() {

        BOOST_LOG(ecc_logger::get()) << "Computing follow set ...";

        // Add end of stack to the follow set of the start grammar
        m_followSet[m_start] = std::make_shared<std::set<std::string>>();
        m_followSet[m_start]->insert(Grammar::END_OF_STACK);

        // Compute the follow set multiple times
        for(int z=0; z <m_productions.size(); z++) {

            // Loop on definitions
            for(auto definition : m_productions) {

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
                            if(m_followSet.count(current) == 0) {
                                m_followSet[current] = std::make_shared<std::set<std::string>>();
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
                                            m_followSet[current]->insert(value);
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
                                if(m_followSet.count(definition.first) == 1) {
                                    m_followSet[current]->insert(m_followSet[definition.first]->begin(),
                                                               m_followSet[definition.first]->end());
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
        for(auto definition : m_productions) {
            std::string leftRecursiveToken = getLeftRecursion(definition.first, std::set<std::string>());
            if (!leftRecursiveToken.empty()) {
                throw std::runtime_error("Left hand side: " + leftRecursiveToken + " has a left recursion");
            }
        }

        // Cond 2
        for(auto definition : m_productions) {
            std::set<std::string> uniqueFirstSetValues;
            for(auto production : *m_productions[definition.first]) {
                for(auto token : *m_productionFirstSet[production]) {
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
        for(auto definition : m_productions) {
            if (m_firstSet[definition.first]->find(Grammar::EPSILON) != m_firstSet[definition.first]->end()) {
                // Get the follow set
                std::shared_ptr<std::set<std::string>> tokenFollowSet = m_followSet[definition.first];
                for (std::string token : *m_firstSet[definition.first]) {
                    if (tokenFollowSet->find(token) != tokenFollowSet->end()) {
                        std::stringstream intersection;
                        std::copy(m_firstSet[definition.first]->begin(), m_firstSet[definition.first]->end(),
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

        for(auto production : *m_productions[token]) {
            for(std::string syntaxToken : *production) {
                if(Grammar::isNonTerminal(syntaxToken)) {

                    // Check recursively for left-recursion
                    std::string result = getLeftRecursion(syntaxToken, visited);
                    if(!result.empty()) {
                        return result;
                    }

                    // Stop checking when the first set of non-terminal does not contain epsilon
                    if(m_firstSet[syntaxToken]->find(Grammar::EPSILON) == m_firstSet[syntaxToken]->end()) {
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
        for(auto &definition : m_productions) {

            // Create map
            m_parseTableMap[definition.first] =
                    std::make_shared<std::map<std::string, std::shared_ptr<std::vector<std::string>>>>();

            // Loop on all productions
            for(auto &production : *definition.second) {

                // Get the first set of the production
                std::shared_ptr<std::set<std::string>> pFirstSet = m_productionFirstSet[production];

                // The map key will always be unique since the grammar
                // passed the LL condition tests

                // Couple each token in the first set to the production, except for epsilon
                bool hasEpsilon = false;
                for(std::string token : *pFirstSet) {
                    if(!Grammar::isEpsilon(token)) {
                        (*m_parseTableMap[definition.first])[token] = production;
                    } else {
                        hasEpsilon = true;
                    }
                }

                // Check if first set contains epsilon, then map the follow set
                if(hasEpsilon) {
                    // Get the follow set of the production
                    std::shared_ptr<std::set<std::string>> pFollowSet = m_followSet[definition.first];

                    // Couple each token in the follow set to the production
                    for(std::string token : *pFollowSet) {
                        (*m_parseTableMap[definition.first])[token] = production;
                    }
                }
            }
        }
    }

    std::shared_ptr<std::vector<std::string>> Grammar::getParseTable(
            const std::string &nonTerminal, const std::string &input) {
        if(m_parseTableMap.find(nonTerminal) != m_parseTableMap.end() &&
                    (*m_parseTableMap[nonTerminal]).find(input) != (*m_parseTableMap[nonTerminal]).end()) {
            return (*m_parseTableMap[nonTerminal])[input];
        }
        return nullptr;
    }

    std::shared_ptr<std::map<std::string, std::shared_ptr<std::vector<std::string>>>> Grammar::getParseMap(
            std::string nonTerminal) {
        if(m_parseTableMap.find(nonTerminal) != m_parseTableMap.end()) {
            return m_parseTableMap[nonTerminal];
        }
        return nullptr;
    }

    std::string Grammar::extractTerminal(std::string terminal) {
        if(terminal.length() > 2 && terminal[0] == '\'' && terminal[terminal.length()-1] == '\'') {
            return terminal.substr(1,terminal.length()-2);
        }
        throw std::runtime_error("String '" + terminal + "' is not a terminal");
    }
}

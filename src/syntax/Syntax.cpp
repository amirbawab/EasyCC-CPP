#include <easycc/Syntax.h>

#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/regex.hpp>
#include <stack>
#include <regex>
#include <boost/algorithm/string.hpp>

#include <iostream>

namespace ecc{

    namespace src = boost::log::sources;
    BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(ecc_logger, src::logger_mt)

    void Syntax::buildFromFiles(std::string stateMachineFile, std::string configFile, std::string errorFile) {
        this->m_grammar = Grammar::buildGrammarFromFile(stateMachineFile);
        this->m_grammar->process();
        this->m_config = SyntaxConfig::buildConfigFromFile(configFile);
        this->m_messages = SyntaxMessages::loadMessagesFromFile(errorFile);
    }

    /**
     * Helper function to get and increment input index
     * @param lexicalToken Vector of lexical tokens pointers
     * @param index Input index
     * @return pointer to a lexical token | nullptr if index out of bound
     */
    std::shared_ptr<LexicalToken> nextToken(
            std::vector<std::shared_ptr<LexicalToken>> &lexicalToken, int &index) {
        // If no more elements
        if(index >= lexicalToken.size())
            return nullptr;
        return lexicalToken[index++];
    }

    void Syntax::parseTokens(std::vector<std::shared_ptr<LexicalToken>> &lexicalTokens,
                             std::vector<std::string> &errorMessages) {

        BOOST_LOG(ecc_logger::get()) << "Started parsing the lexical tokens ...";

        for(int phase = 1; phase <= this->m_config->getParsingPhases(); ++phase) {

            // Prepare the stack
            std::stack<std::string> parseStack;

            // Keep track if parser is in panic mode
            bool stable = true;

            // Store the current index of the lexical token
            int inputIndex = 0;

            // Store lexical tokens
            std::shared_ptr<LexicalToken> lexicalToken = nextToken(lexicalTokens, inputIndex);

            // Add the end of stack
            parseStack.push(Grammar::END_OF_STACK);

            // Add grammar start
            parseStack.push(m_grammar->getStart());

            // While more non-terminals are in the parse stack
            while(parseStack.top() != Grammar::END_OF_STACK) {

                // Get the top token from the parser stack
                const std::string &top = parseStack.top();

                // Check the type of the token
                if(Grammar::isTerminal(top)) {

                    // If there is a match
                    if(Grammar::extractTerminal(top) == lexicalToken->getName()) {

                        // Start over by scanning new input and processing a new top
                        parseStack.pop();
                        lexicalToken = nextToken(lexicalTokens, inputIndex);

                        // Mark stable
                        stable = true;
                    } else {
                        throw std::runtime_error(
                                "Failed to process the input: "
                                        "In the Syntax analysis phase, the stack top terminal "
                                        "and the lexical input terminal token did not match! "
                                        "Please report this problem.");
                    }
                } else if(Grammar::isSemanticAction(top)) {

                    // Call the semantic action handler.
                    // inputIndex value is subtracted by 2:
                    // inputIndex:   -1-+-2-+-3-+-3-+-4-
                    //                  A   B   #C  $
                    // To retrieve the value of `B`, the semantic action `#C`
                    // is placed right after. But at the time `#C` is processed,
                    // the value of inputIndex is 3 which is the original index of B + 2.
                    // Note that if the semantic action is placed at the beginning
                    // of the grammar production, then the input index will be 2 - 1 = -1
                    this->m_semanticAction(top, phase, lexicalTokens, inputIndex-2, stable);

                    // Remove the action from the stack
                    parseStack.pop();

                } else { // It is a non-terminal

                    // Get record from the parse table
                    std::shared_ptr<std::vector<std::string>> production =
                            m_grammar->getParseTable(top, lexicalToken->getName());

                    // Check if the record exists or it is an error
                    if(production) {
                        parseStack.pop();

                        // Insert the new production right to left
                        for(auto i=0; i < production->size(); ++i) {

                            // If not epsilon
                            if(!Grammar::isEpsilon((*production)[production->size()-1-i])){
                                parseStack.push((*production)[production->size()-1-i]);
                            }
                        }

                        // Mark stable
                        stable = true;

                    } else { // Error found

                        // Generate error message in the first parsing phase
                        if(phase == 1) {
                            errorMessages.push_back(generateErrorMessage(top, lexicalTokens, inputIndex-1));
                        }

                        // If terminal is in the follow set or there is no more input to process,
                        // then pop the parse stack
                        std::shared_ptr<std::set<std::string>> followSet = m_grammar->getFollowSet(top);
                        if(followSet->find(lexicalToken->getName()) != followSet->end() ||
                           lexicalToken->getName() == Grammar::END_OF_STACK) {
                            parseStack.pop();
                        } else {
                            lexicalToken = nextToken(lexicalTokens, inputIndex);
                        }

                        // Mark unstable
                        stable = false;
                    }
                }
            }

            if(lexicalToken->getName() != Grammar::END_OF_STACK) {
                // Generate error message in the first parsing phase
                if(phase == 1) {
                    errorMessages.push_back(generateErrorMessage(
                            parseStack.top(), lexicalTokens, inputIndex-1));
                }
            }
        }

        BOOST_LOG(ecc_logger::get()) << "Finished parsing the lexical tokens ...";
    }

    std::string Syntax::generateErrorMessage(std::string nonTerminal,
                                             std::vector<std::shared_ptr<LexicalToken>> &lexicalTokens,
                                             int index) {
        // Load error message
        std::string message = m_messages->getErrorMessage(nonTerminal, lexicalTokens[index]->getName());
        std::string messageCopy = message;

        // Match error messages
        std::regex exp("(\\$\\{lexical(\\.(?:next|previous))*\\.(?:value|name|line|column)\\})");
        std::smatch match;

        while(std::regex_search(messageCopy, match, exp)) {

            // Split by dot
            std::vector<std::string> words;
            std::string matchValue = match[0].str();
            boost::split(words, matchValue, boost::is_any_of("."), boost::token_compress_on);

            // Navigate to the correct token
            int newIndex = index;
            for(size_t i=1; i < words.size()-1; i++) {
                if(words[i] == "next") {
                    newIndex++;
                } else if(words[i] == "previous") {
                    newIndex--;
                }
            }

            std::string newValue;

            // If new index is not found
            if(newIndex < 0 || newIndex >= lexicalTokens.size()) {
                newValue = "undefined";
            } else {
                std::string type = words[words.size() - 1].substr(0, words[words.size() - 1].length() - 1);
                if (type == "value") {
                    newValue = lexicalTokens[newIndex]->getValue();
                } else if (type == "name") {
                    newValue = lexicalTokens[newIndex]->getName();
                } else if (type == "line") {
                    newValue = std::to_string(lexicalTokens[newIndex]->getLine());
                } else if (type == "column") {
                    newValue = std::to_string(lexicalTokens[newIndex]->getColumn());
                }
            }

            boost::replace_all(message, matchValue, newValue);
            messageCopy = match.suffix();
        }

        return message;
    }
}

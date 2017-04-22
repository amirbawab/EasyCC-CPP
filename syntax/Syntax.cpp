#include "Syntax.h"

#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <stack>

#include <iostream>

namespace ecc{

    namespace src = boost::log::sources;
    BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(ecc_logger, src::logger_mt)

    Syntax::Syntax(std::string grammarFile, std::string messagesFileName) {
        this->grammar = std::make_shared<Grammar>(grammarFile);
        this->messages = SyntaxMessages::loadMessages(messagesFileName);
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

    void Syntax::parseTokens(std::vector<std::shared_ptr<LexicalToken>> &lexicalTokens) {

        BOOST_LOG(ecc_logger::get()) << "Started parsing the lexical tokens ...";

        // Prepare the stack
        std::stack<std::string> parseStack;

        // Keep track if any error occurred
        bool error = false;

        // Keep track if parser is in panic mode
        bool stable = true;

        // Store the current index of the lexical token
        int inputIndex = 0;

        // Store lexical tokens
        std::shared_ptr<LexicalToken> lexicalToken = nextToken(lexicalTokens, inputIndex);

        // Add the end of stack
        parseStack.push(Grammar::END_OF_STACK);

        // Add grammar start
        parseStack.push(grammar->getStart());

        // While more non-terminals are in the parse stack
        while(parseStack.top() != Grammar::END_OF_STACK) {

            // Get the top token from the parser stack
            std::string top = parseStack.top();

            // Check the type of the token
            if(Grammar::isTerminal(top)) {

                // If there is a match
                if(Grammar::extractTerminal(top) == lexicalToken->getName()) {

                    // Start over by scanning new input and processing a new top
                    parseStack.pop();
                    lexicalToken = nextToken(lexicalTokens, inputIndex);

                } else {
                    throw std::runtime_error(
                            "Failed to process the input: "
                            "In the Syntax analysis phase, the stack top terminal "
                            "and the lexical input terminal token did not match! "
                            "Please report this problem.");
                }
            } else { // It is a non-terminal

                // Get record from the parse table
                std::shared_ptr<std::vector<std::string>> production =
                        grammar->getParseTabel(top, lexicalToken->getName());

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
                } else { // Error found

                    // Load error message
                    std::string message = messages->getErrorMessage(top, lexicalToken->getName());
                    std::cout << message << std::endl;

                    // If terminal is in the follow set or it is the End of the stack, then pop stack
                    std::shared_ptr<std::set<std::string>> firstSet = grammar->getFirstSet(top);
                    if(firstSet->find(lexicalToken->getName()) != firstSet->end() ||
                            lexicalToken->getName() == Grammar::END_OF_STACK) {
                        parseStack.pop();
                    } else {
                        lexicalToken = nextToken(lexicalTokens, inputIndex);
                    }
                }
            }
        }
    }
}

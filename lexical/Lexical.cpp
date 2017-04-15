#include "Lexical.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>

namespace ecc {
    Lexical::Lexical(std::string stateMachineFileName, std::string configFileName, std::string messagesFileName) {
        this->graph = Graph::buildGraph(stateMachineFileName);
        this->config = Config::buildConfig(configFileName);
        this->messages = Messages::loadMessages(messagesFileName);
    }

    void Lexical::generateLexicalTokens(
            std::string fileName,
            std::vector<std::shared_ptr<LexicalToken>> &lexicalVector,
            std::vector<std::string> &errorMessages) {

        // Keep track of file information
        int tokenLine = 1;
        int tokenColumn = 1;
        int tokenPosition = 0;
        int line = 1;
        int column = 1;
        int position = 0;

        // Keep track of the state
        std::shared_ptr<State> state = graph->getInitialState();

        // Keep track of the token value created
        std::stringstream tokenValueStream;

        // Read character by character
        char ch;
        std::fstream fin(fileName, std::fstream::in);
        while (fin >> std::noskipws >> ch) {
            // Backtrack if needed
            bool backtrack = true;

            // Must be updated by the states
            while(backtrack) {

                // If initial state, update token position
                if(state->getType() == State::INITIAL) {
                    tokenLine = line;
                    tokenColumn = column;
                    tokenPosition = position;
                }

                // Jump to the next state
                state = graph->getStateById(graph->getStateOnRead(state->getId(), ch));

                // If new state is a final state
                if (state->getType() == State::FINAL) {

                    // Update backtrack according to the state data
                    backtrack = state->mustBacktrack();

                    // If shouldn't backtrack, then the current char is part of the token value
                    if(!backtrack) {
                        tokenValueStream << ch;
                    }

                    // Create token
                    std::shared_ptr<LexicalToken> token =createToken(state->getTokenName(), tokenValueStream.str(),
                                                                     tokenLine, tokenColumn, tokenPosition);

                    // If token created
                    if(token) {
                        // Add to the vector of lexical token
                        lexicalVector.push_back(token);

                        // Check if the token is an error token
                        if(token->getType() == LexicalToken::Type::ERROR_TOKEN) {
                            errorMessages.push_back(generateErrorMessage(token));
                        }
                    }

                    // Reset values
                    tokenValueStream.str(std::string());
                    state = graph->getInitialState();
                } else {

                    // Non-final states does need to backtrack
                    backtrack = false;

                    // Add current read character as part of the token value
                    if(state->getType() == State::NORMAL) {
                        tokenValueStream << ch;
                    }
                }
            }

            // TODO Add "Config::AUTO" to automatically detect
            // Check if the read char is a new line
            if(config->getNewLine() == Config::LF && ch == '\n' ||
               config->getNewLine() == Config::CR && ch == '\r') {
                column = 1;
                line++;
            } else if(config->getNewLine() == Config::CRLF && ch == '\r' && fin.peek() == '\n') {
                // Skip next character
                fin >> std::noskipws >> ch;
                column = 1;
                line++;
            } else {
                column++;
            }

            position++;
        }

        // One more call for the end of file
        state = graph->getStateById(graph->getStateOnRead(state->getId(), EOF));

        // After reaching EOF, the current state should be either INITIAL or FINAL
        if(state->getType() == State::NORMAL) {
            throw std::runtime_error("Not all input was parsed. Please verify that the "
                                             "state machine lands on a final state "
                                             "when reaching EOF. "
                                             "Error while parsing: "+
                                             tokenValueStream.str());
        }

        // If final state, then create the last token
        if(state->getType() == State::FINAL) {
            std::shared_ptr<LexicalToken> token = createToken(state->getTokenName(), tokenValueStream.str(),
                                                              tokenLine, tokenColumn, tokenPosition);

            // If token created
            if(token) {
                // Add to the vector of lexical token
                lexicalVector.push_back(token);

                // Check if the token is an error token
                if(token->getType() == LexicalToken::Type::ERROR_TOKEN) {
                    errorMessages.push_back(generateErrorMessage(token));
                }
            }
        }

        // Add final token
        lexicalVector.push_back(std::make_shared<LexicalToken>(
                LexicalToken::Type::NORMAL_FINAL_TOKEN, config->getEndOfFile(),
                config->getEndOfFile(), line, column, position));
    }

    /**
     * Create and customize a token based on the lexical configuration
     * @return object | null if the token should be ignored
     */
    std::shared_ptr<LexicalToken> Lexical::createToken(
            std::string tokenName, std::string tokenValue, const int &line, const int &column, const int &position) {

        // Check the type of the token name
        if(config->isErrorToken(tokenName)) {
            return std::make_shared<LexicalToken>(
                    LexicalToken::Type::ERROR_TOKEN, tokenName,
                    tokenValue, line, column, position);
        } else if(!config->mustIgnoreToken(tokenName)) {
            tokenName = config->updateTokenName(tokenName, tokenValue);
            return std::make_shared<LexicalToken>(
                    LexicalToken::Type::NORMAL_TOKEN, tokenName,
                    tokenValue, line, column, position);
        }
        return nullptr;
    }

    std::string Lexical::generateErrorMessage(std::shared_ptr<LexicalToken> lexicalToken) {
        std::string message = this->messages->getErrorMessage(lexicalToken->getName());
        boost::replace_all(message, "${value}", lexicalToken->getValue());
        boost::replace_all(message, "${line}", std::to_string(lexicalToken->getLine()));
        boost::replace_all(message, "${column}", std::to_string(lexicalToken->getColumn()));
        return message;
    }
}

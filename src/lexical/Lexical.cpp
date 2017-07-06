#include <easycc/Lexical.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>

#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(ecc_logger, src::logger_mt)

namespace ecc {

    void Lexical::buildFromFiles(std::string stateMachineFile, std::string configFile, std::string errorFile){
        this->m_graph = Graph::buildGraphFromFile(stateMachineFile);
        this->m_config = LexicalConfig::buildConfigFromFile(configFile);
        this->m_messages = LexicalMessages::loadMessagesFromFile(errorFile);
    }

    void Lexical::buildFromStrings(std::string &stateMachine, std::string &config, std::string &errors) {
        this->m_graph = Graph::buildGraphFromString(stateMachine);
        this->m_config = LexicalConfig::buildConfigFromString(config);
        this->m_messages = LexicalMessages::loadMessagesFromString(errors);
    }

    bool Lexical::generateLexicalTokens(
            std::string fileName,
            std::vector<std::shared_ptr<LexicalToken>> &lexicalVector) {
        BOOST_LOG(ecc_logger::get()) << "Converting content of " << fileName << " into lexical tokens ...";

        // Keep track of file information
        int tokenLine = 1;
        int tokenColumn = 1;
        int line = 1;
        int column = 1;
        bool success = true;

        // Keep track of the state
        std::shared_ptr<State> state = m_graph->getInitialState();

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
                }

                // Jump to the next state
                state = m_graph->getStateById(m_graph->getStateOnRead(state->getId(), ch));

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
                                                                     tokenLine, tokenColumn);

                    // If token created
                    if(token) {
                        // Add to the vector of lexical token
                        lexicalVector.push_back(token);

                        // Check if the token is an error token
                        if(token->getType() == LexicalToken::Type::ERROR_TOKEN) {
                            std::cerr << fileName << ": " << generateErrorMessage(token) << std::endl;
                            success = false;
                        }
                    }

                    // Reset values
                    tokenValueStream.str(std::string());
                    state = m_graph->getInitialState();
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
            if(m_config->getNewLine() == LexicalConfig::LF && ch == '\n' ||
               m_config->getNewLine() == LexicalConfig::CR && ch == '\r') {
                column = 1;
                line++;
            } else if(m_config->getNewLine() == LexicalConfig::CRLF && ch == '\r' && fin.peek() == '\n') {
                // Skip next character
                fin >> std::noskipws >> ch;
                column = 1;
                line++;
            } else {
                column++;
            }
        }

        // One more call for the end of file
        state = m_graph->getStateById(m_graph->getStateOnRead(state->getId(), EOF));

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
                                                              tokenLine, tokenColumn);

            // If token created
            if(token) {
                // Add to the vector of lexical token
                lexicalVector.push_back(token);

                // Check if the token is an error token
                if(token->getType() == LexicalToken::Type::ERROR_TOKEN) {
                    std::cerr << generateErrorMessage(token) << std::endl;
                    success = false;
                }
            }
        }

        // Add final token
        lexicalVector.push_back(std::make_shared<LexicalToken>(
                LexicalToken::Type::NORMAL_FINAL_TOKEN, LexicalToken::END_OF_FILE,
                LexicalToken::END_OF_FILE, line, column));

        // Log generate tokens
        BOOST_LOG(ecc_logger::get()) << "Finished generating lexical tokens:";
        if(logging::core::get()->get_logging_enabled()) {
            for(int i=0; i < lexicalVector.size(); i++) {
                BOOST_LOG(ecc_logger::get()) << lexicalVector[i]->getString();
            }
        }
        BOOST_LOG(ecc_logger::get()) << (success ? "SUCCESS" : "FAILURE");
        BOOST_LOG(ecc_logger::get()) << "----------";
        return success;
    }

    /**
     * Create and customize a token based on the lexical configuration
     * @return object | null if the token should be ignored
     */
    std::shared_ptr<LexicalToken> Lexical::createToken(
            std::string tokenName, std::string tokenValue, const int &line, const int &column) {

        // Check the type of the token name
        if(m_config->isErrorToken(tokenName)) {
            return std::make_shared<LexicalToken>(
                    LexicalToken::Type::ERROR_TOKEN, tokenName,
                    tokenValue, line, column);
        } else if(!m_config->mustIgnoreToken(tokenName)) {
            tokenName = m_config->updateTokenName(tokenName, tokenValue);
            return std::make_shared<LexicalToken>(
                    LexicalToken::Type::NORMAL_TOKEN, tokenName,
                    tokenValue, line, column);
        }
        return nullptr;
    }

    std::string Lexical::generateErrorMessage(std::shared_ptr<LexicalToken> lexicalToken) {
        std::string message = this->m_messages->getErrorMessage(lexicalToken->getName());
        boost::replace_all(message, "${value}", lexicalToken->getValue());
        boost::replace_all(message, "${line}", std::to_string(lexicalToken->getLine()));
        boost::replace_all(message, "${column}", std::to_string(lexicalToken->getColumn()));
        return message;
    }
}

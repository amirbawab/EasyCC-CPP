#ifndef LEXICAL_LEXICAL_H_
#define LEXICAL_LEXICAL_H_

#include <memory>
#include <string>
#include <vector>
#include "graph/Graph.h"
#include "config/Config.h"
#include "token/LexicalToken.h"
#include "config/Messages.h"

namespace ecc {

    class Lexical {
    public:
        Lexical(std::string stateMachineFileName, std::string configFileName, std::string messagesFileName);

        /**
         * Analyze a text file
         * @param fileName Text file to be analyzed
         * @param vector which will contain the tokens
         * @param vector which will contain the error tokens
         */
        void generateLexicalTokens(std::string fileName,
                                   std::vector<std::shared_ptr<LexicalToken>> &lexicalVector,
                                   std::vector<std::string> &errorMessages);
    private:

        // State machine
        std::shared_ptr<Graph> m_graph;

        // Tokens configuration
        std::shared_ptr<Config> m_config;

        // Messages
        std::shared_ptr<Messages> m_messages;

       /**
        * Create a lexical token
        * @param token name
        * @param token value
        * @param line
        * @param column
        * @param position
        */
        std::shared_ptr<LexicalToken> createToken(
               std::string tokenName, std::string tokenValue, const int &line, const int &column, const int &position);

        /**
         * Generate an error message based on the error token name
         * @param pointer to the lexical token
         * @return error message
         */
        std::string generateErrorMessage(std::shared_ptr<LexicalToken> lexicalToken);
    };
}

#endif

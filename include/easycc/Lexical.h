#ifndef LEXICAL_LEXICAL_H_
#define LEXICAL_LEXICAL_H_

#include <memory>
#include <string>
#include <vector>
#include "Graph.h"
#include "LexicalConfig.h"
#include "LexicalToken.h"
#include "LexicalMessages.h"

namespace ecc {

    class Lexical {
    public:

        /**
         * Setters for the main configuration files
         */
        void setStateMachineFile(std::string file);
        void setStateMachineString(std::string data);
        void setConfigFile(std::string file);
        void setConfigString(std::string data);
        void setMessagesFile(std::string file);
        void setMessagesString(std::string data);

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
        std::shared_ptr<LexicalConfig> m_config;

        // Messages
        std::shared_ptr<LexicalMessages> m_messages;

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

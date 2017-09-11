#ifndef LEXICAL_LEXICAL_H_
#define LEXICAL_LEXICAL_H_

#include <memory>
#include <string>
#include <vector>
#include <easycc/Graph.h>
#include <easycc/LexicalConfig.h>
#include <easycc/LexicalToken.h>
#include <easycc/LexicalMessages.h>

namespace ecc {

    class Lexical {
    public:

        /**
         * Build lexical members from files
         */
        void buildFromFiles(std::string stateMachineFile, std::string configFile, std::string errorFile);

        /**
         * Build lexical members from data
         */
        void buildFromStrings(std::string &stateMachine, std::string &config, std::string &errors);

        /**
         * Analyze a text file
         * @param fileName Text file to be analyzed
         * @param vector which will contain the tokens
         */
        bool generateLexicalTokens(std::string fileName,
                                   std::vector<std::shared_ptr<LexicalToken>> &lexicalVector);
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
        */
        std::shared_ptr<LexicalToken> createToken(
               std::string tokenName, std::string tokenValue, const int &line, const int &column);

        /**
         * Generate an error message based on the error token name
         * @param pointer to the lexical token
         * @return error message
         */
        std::string generateErrorMessage(std::string fileName, std::shared_ptr<LexicalToken> lexicalToken);
    };
}

#endif

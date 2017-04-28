#ifndef EASYCC_CPP_SYNTAX_H
#define EASYCC_CPP_SYNTAX_H

#include "grammar/Grammar.h"
#include "../lexical/token/LexicalToken.h"
#include "config/SyntaxMessages.h"
#include "config/SyntaxConfig.h"
#include <string>
#include <memory>
#include <functional>

namespace ecc{
    class Syntax {
    public:
        Syntax(std::string grammarFile, std::string configFileName, std::string messagesFileName);

        /**
         * Parse lexical tokens
         * @param lexicalTokens
         */
        void parseTokens(std::vector<std::shared_ptr<LexicalToken>> &lexicalTokens,
                         std::vector<std::string> &errorMessages);

        /**
         * Set semantic action
         * @param semanticAction Function to execute on semantic action calls
         */
        void setSemanticAction(std::function<void
                (std::string, int, std::vector<std::shared_ptr<LexicalToken>>&, int)> semanticAction) {
            this->m_semanticAction = semanticAction;
        }
    private:
        std::shared_ptr<Grammar> m_grammar;
        std::shared_ptr<SyntaxConfig> m_config;
        std::shared_ptr<SyntaxMessages> m_messages;
        std::function<void(std::string, int, std::vector<std::shared_ptr<LexicalToken>>&, int)> m_semanticAction;

        /**
         * Generate an error message
         * @param nonTerminal A non terminal string
         * @param pointer to the lexical token
         * @param index Index of the current lexical token
         * @return error message
         */
        std::string generateErrorMessage(std::string nonTerminal,
                                         std::vector<std::shared_ptr<LexicalToken>> &lexicalToken,
                                         int index);
    };
}


#endif

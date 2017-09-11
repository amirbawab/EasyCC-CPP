#ifndef EASYCC_CPP_SYNTAX_H
#define EASYCC_CPP_SYNTAX_H

#include <Grammar.h>
#include <LexicalToken.h>
#include <SyntaxMessages.h>
#include <string>
#include <memory>
#include <functional>

namespace ecc{
    class Syntax {
    public:

        /**
         * Build syntax members from files
         */
        void buildFromFiles(std::string grammarFile, std::string errorFile);

        /**
         * Build syntax members from strings
         */
        void buildFromStrings(std::string grammar, std::string error);

        /**
         * Parse lexical tokens
         * @param lexicalTokens
         */
        bool parseTokens(std::string fileName, std::vector<std::shared_ptr<LexicalToken>> &lexicalTokens);

        /**
         * Set semantic action
         * @param semanticAction Function to execute on semantic action calls
         */
        void setSemanticAction(std::function<void
                (std::string, int, std::vector<std::shared_ptr<LexicalToken>>&, int)> semanticAction) {
            this->m_semanticAction = semanticAction;
        }

        /**
         * Set on syntax error
         * @param onSyntaxError
         */
        void setOnSyntaxError(std::function<void()> onSyntaxError) {
            this->m_onSyntaxError = onSyntaxError;
        }

        /**
         * Silent mode ignores syntax error messages
         * @param silent
         */
        void setSilentSyntaxErrorMessages(bool silent) {
            this->m_silentSyntaxErrorMessages = silent;
        }

        /**
         * Silent semantic actions won't call corresponding handlers
         * @param silent
         */
        void setSilentSemanticAction(bool silent) {
            this->m_silentSemanticEvents = silent;
        }

        /**
         * Set phase number
         * @param phase
         */
        void setParsingPhase(int phase) {
            this->m_phase = phase;
        }
    private:
        int m_phase = 0;
        bool m_silentSyntaxErrorMessages = false;
        bool m_silentSemanticEvents = false;
        std::shared_ptr<Grammar> m_grammar;
        std::shared_ptr<SyntaxMessages> m_messages;
        std::function<void(std::string, int, std::vector<std::shared_ptr<LexicalToken>>&, int)> m_semanticAction;
        std::function<void()> m_onSyntaxError;

        /**
         * Generate an error message
         * @param nonTerminal A non terminal string
         * @param pointer to the lexical token
         * @param index Index of the current lexical token
         * @return error message
         */
        std::string generateErrorMessage(std::string fileName, std::string nonTerminal,
                                         std::vector<std::shared_ptr<LexicalToken>> &lexicalToken,
                                         int index);
    };
}


#endif

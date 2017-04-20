#ifndef EASYCC_CPP_SYNTAX_H
#define EASYCC_CPP_SYNTAX_H

#include "grammar/Grammar.h"
#include "../lexical/token/LexicalToken.h"
#include "config/SyntaxMessages.h"
#include <string>
#include <memory>

namespace ecc{
    class Syntax {
    public:
        Syntax(std::string grammarFile, std::string messagesFileName);
        void parseTokens(std::vector<std::shared_ptr<LexicalToken>> &lexicalTokens);
    private:
        std::shared_ptr<Grammar> grammar;
        std::shared_ptr<SyntaxMessages> messages;
    };
}


#endif

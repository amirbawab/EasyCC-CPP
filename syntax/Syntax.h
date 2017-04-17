#ifndef EASYCC_CPP_SYNTAX_H
#define EASYCC_CPP_SYNTAX_H

#include "grammar/Grammar.h"
#include "../lexical/token/LexicalToken.h"
#include <string>
#include <memory>

namespace ecc{
    class Syntax {
    public:
        Syntax(std::string grammarFile);
        void parseTokens(std::vector<std::shared_ptr<LexicalToken>> &lexicalTokens);
    private:
        std::shared_ptr<Grammar> grammar;
    };
}


#endif

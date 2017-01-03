#ifndef EASYCC_CPP_SYNTAX_H
#define EASYCC_CPP_SYNTAX_H

#include "grammar/Grammar.h"
#include <string>
#include <memory>

namespace ecc{
    class Syntax {
    public:
        Syntax(std::string grammarFile);
    private:
        std::shared_ptr<Grammar> grammar;
    };
}


#endif

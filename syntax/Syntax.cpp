#include "Syntax.h"

namespace ecc{
    Syntax::Syntax(std::string grammarFile) {
        grammar = std::make_shared<Grammar>(grammarFile);


    }
}

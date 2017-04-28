#ifndef EASYCC_CPP_SEMANTIC_H
#define EASYCC_CPP_SEMANTIC_H

#include <string>
#include <vector>
#include <memory>
#include "../lexical/token/LexicalToken.h"

namespace ecc {
    class Semantic {
    public:
        void handle(std::string semanticAction, int phase,
                    std::vector<std::shared_ptr<LexicalToken>> &lexicalTokens, int index);
    };
}

#endif

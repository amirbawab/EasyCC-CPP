#include "Semantic.h"
#include <iostream>

namespace ecc {
    void Semantic::handle(std::string semanticAction, int phase,
                          std::vector<std::shared_ptr<LexicalToken>> &lexicalTokens, int index) {
        std::cout << semanticAction << " fired!" << std::endl;
    }
}
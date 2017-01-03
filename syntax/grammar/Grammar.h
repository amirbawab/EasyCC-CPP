#ifndef EASYCC_CPP_GRAMMAR_H
#define EASYCC_CPP_GRAMMAR_H

#include <string>
#include <map>
#include <vector>

namespace ecc {
    class Grammar {
    public:
        Grammar(std::string grammarFile);
        static bool isTerminal();
        static bool isNonTerminal();
    private:
        std::map<std::string,std::vector<std::vector<std::string>>> production;
    };
}


#endif //EASYCC_CPP_GRAMMAR_H

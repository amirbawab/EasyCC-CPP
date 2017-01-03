#ifndef EASYCC_CPP_GRAMMAR_H
#define EASYCC_CPP_GRAMMAR_H

#include <string>
#include <map>
#include <vector>

namespace ecc {
    class Grammar {
    public:
        Grammar(std::string grammarFile);
        static bool isTerminal(std::string token);
        static bool isNonTerminal(std::string token);
    private:
        std::map<std::string,std::vector<std::vector<std::string>>> production;
        void parseGrammar();
    };
}


#endif //EASYCC_CPP_GRAMMAR_H

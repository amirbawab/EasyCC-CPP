#ifndef EASYCC_CONVERTGRAMMAR_H
#define EASYCC_CONVERTGRAMMAR_H

#include <string>
#include <easycc/Grammar.h>
#include <easycc/tools/Convertible.h>

namespace ecc {
    class ConvertGrammar : public Convertible {
    public:
        void convert(std::string fileName);
    private:
        void generateFirstSet(std::stringstream &stream, Grammar &grammar);
        void generateFollowSet(std::stringstream &stream, Grammar &grammar);
        void generateParseTable(std::stringstream &stream, Grammar &grammar);
    };
}

#endif

#ifndef EASYCC_CONVERTGRAMMAR_H
#define EASYCC_CONVERTGRAMMAR_H

#include <string>
#include <easycc/Grammar.h>
#include <easycc/tools/Convertible.h>

namespace ecc {
    class ConvertGrammar : public Convertible {
    public:
        void convert(std::string fileName);
        static const std::string FIRST_SET_PATTERN;
        static const std::string FOLLOW_SET_PATTERN;
        static const std::string PARSE_TABLE_PATTERN;
    private:
        void generateFirstSet(std::stringstream &stream, Grammar &grammar);
        void generateFollowSet(std::stringstream &stream, Grammar &grammar);
        void generateParseTable(std::stringstream &stream, Grammar &grammar);
    };
}

#endif

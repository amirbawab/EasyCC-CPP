#ifndef EASYCC_CONVERTFF_H
#define EASYCC_CONVERTFF_H

#include <string>
#include <easycc/Grammar.h>

namespace ecc {
    class ConvertFF {
    public:
        void init(int argc, char *argv[]);
        void convert(std::string fileName);
        void printUsage();
        std::string getOutputFile() const { return m_outputFile;}
        std::string getInputFile() const { return m_inputFile;}
    private:
        std::string m_outputFile;
        std::string m_inputFile;
        void initParams(int argc, char *argv[]);
        void generateFirstSet(std::stringstream &stream, Grammar &grammar);
        void generateFollowSet(std::stringstream &stream, Grammar &grammar);
        void generateParseTable(std::stringstream &stream, Grammar &grammar);
    };
}

#endif

#ifndef EASYCC_CONVERTFF_H
#define EASYCC_CONVERTFF_H

#include <string>

namespace ecc {
    class ConvertFF {
    public:
        void init(int argc, char *argv[]);
        void convert(std::string fileName);
        void printUsage();
        std::string getOutputFile() const { return m_outputFile;}
    private:
        void initParams(int argc, char *argv[]);
        std::string m_outputFile;
    };
}

#endif

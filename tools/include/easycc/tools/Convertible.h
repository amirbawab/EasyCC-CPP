#ifndef EASYCC_CONVERTIBLE_H
#define EASYCC_CONVERTIBLE_H

#include <string>

namespace ecc{
    class Convertible {
    public:
        static const int RETURN_SUCCESS = 0;
        static const int RETURN_ERROR = 1;
        virtual void convert(std::string fileName)=0;
        int init(int argc, char *argv[]);
        int run();
        void printUsage(std::string header, std::string footer);
    protected:
        std::string m_outputFile;
        std::string m_inputFile;
        std::string m_templateFile;
        std::string m_newContent;
    private:
        void initParams(int argc, char *argv[]);
    };
}

#endif

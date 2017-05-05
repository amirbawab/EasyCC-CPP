#ifndef EASYCC_CONVERTIBLE_H
#define EASYCC_CONVERTIBLE_H

#include <string>
#include <vector>

namespace ecc{
    class ConvertResources {
    public:
        static const int RETURN_SUCCESS = 0;
        static const int RETURN_ERROR = 1;
        int convert();
        int run(int argc, char *argv[]);
        void printUsage();
    protected:
        std::string m_outputFile;
        std::vector<std::string> m_patternJson;
        std::string m_templateFile;
    private:
        void initParams(int argc, char *argv[]);
    };
}

#endif

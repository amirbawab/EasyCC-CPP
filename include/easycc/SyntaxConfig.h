#ifndef EASYCC_CPP_SYNTAXCONFIG_H
#define EASYCC_CPP_SYNTAXCONFIG_H

#include <memory>
#include <string>
#include <set>
#include <map>

namespace ecc {
    class SyntaxConfig {
    public:

        /**
         * Create a SyntaxConfig based on a JSON file
         * @param config file name
         * @return pointer to SyntaxConfig object
         */
        static std::shared_ptr<SyntaxConfig> buildConfig(std::string configFileName);

        int getParsingPhases() { return this->m_parsingPhases;}
    private:
        int m_parsingPhases;
    };
}

#endif

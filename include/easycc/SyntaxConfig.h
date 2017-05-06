#ifndef EASYCC_CPP_SYNTAXCONFIG_H
#define EASYCC_CPP_SYNTAXCONFIG_H

#include <memory>
#include <string>
#include <set>
#include <map>
#include <rapidjson/document.h>

namespace ecc {
    class SyntaxConfig {
    public:

        /**
         * Create a SyntaxConfig based on a JSON file
         * @param config file name
         * @return pointer to SyntaxConfig object
         */
        static std::shared_ptr<SyntaxConfig> buildConfigFromFile(std::string configFileName);

        /**
         * Create a SyntaxConfig based on a JSON data
         * @param data JSON data
         * @return pointer to SyntaxConfig object
         */
        static std::shared_ptr<SyntaxConfig> buildConfigFromString(std::string data);

        int getParsingPhases() { return this->m_parsingPhases;}
    private:
        int m_parsingPhases;

        /**
         * Create a SyntaxConfig
         * @param data JSON data
         * @return pointer to SyntaxConfig object
         */
        static std::shared_ptr<SyntaxConfig> buildConfig(rapidjson::Document &document);
    };
}

#endif

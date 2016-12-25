#ifndef EASYCC_CPP_CONFIG_H
#define EASYCC_CPP_CONFIG_H

#include <memory>
#include <string>
#include <vector>
#include <map>

namespace ecc {
    class Config {
    public:
        static std::shared_ptr<Config> buildConfig(std::string configFileName);
    private:

        // Token names ignored
        std::string ignorePrefix;
        std::string ignoreSuffix;
        std::vector<std::string> ignoreInclude;
        std::vector<std::string> ignoreExclude;

        // Token names classified as error tokens
        std::string errorPrefix;
        std::string errorSuffix;
        std::vector<std::string> errorInclude;
        std::vector<std::string> errorExclude;

        // Reserved token names
        std::map<std::string, std::map<std::string, std::string>> reservedTokens;
    };
}

#endif

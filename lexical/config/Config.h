#ifndef EASYCC_CPP_CONFIG_H
#define EASYCC_CPP_CONFIG_H

#include <memory>
#include <string>
#include <set>
#include <map>

namespace ecc {
    class Config {
    public:
        /**
         * Create a Config based on a JSON file
         * @param config file name
         * @return pointer to Config object
         */
        static std::shared_ptr<Config> buildConfig(std::string configFileName);

        /**
         * Check if token must be ignored
         * @param token name
         * @return true|false
         */
        bool mustIgnoreToken(std::string tokenName);

        /**
         * Check if the token is an error token
         * @param token name
         * @return true|false
         */
        bool isErrorToken(std::string tokenName);



        /**
         * Verify if token value is reserved by another token name
         *
         */
        std::string updateTokenName(std::string tokenName, std::string tokenValue);
    private:

        // Token names ignored
        std::string ignorePrefix;
        std::string ignoreSuffix;
        std::set<std::string> ignoreInclude;
        std::set<std::string> ignoreExclude;

        // Token names classified as error tokens
        std::string errorPrefix;
        std::string errorSuffix;
        std::set<std::string> errorInclude;
        std::set<std::string> errorExclude;

        // Reserved token names
        std::map<std::string, std::map<std::string, std::string>> reservedTokens;
    };
}

#endif

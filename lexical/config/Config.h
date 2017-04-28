#ifndef EASYCC_CPP_CONFIG_H
#define EASYCC_CPP_CONFIG_H

#include <memory>
#include <string>
#include <set>
#include <map>

namespace ecc {
    class Config {
    public:

        // Line separators
        static const std::string LF;
        static const std::string CR;
        static const std::string CRLF;

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
         * Get new line symbol
         * @return new line symbol
         */
        std::string getNewLine() const {
            return this->newLine;
        }

        /**
         * Verify if token value is reserved by another token name
         *
         */
        std::string updateTokenName(std::string tokenName, std::string tokenValue);
    private:

        // Define new line
        std::string newLine;

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

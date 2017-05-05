#ifndef EASYCC_CPP_LEXICALCONFIG_H
#define EASYCC_CPP_LEXICALCONFIG_H

#include <memory>
#include <string>
#include <set>
#include <map>

namespace ecc {
    class LexicalConfig {
    public:

        // Line separators
        static const std::string LF;
        static const std::string CR;
        static const std::string CRLF;

        /**
         * Create a LexicalConfig based on a JSON file
         * @param config file name
         * @return pointer to LexicalConfig object
         */
        static std::shared_ptr<LexicalConfig> buildConfigFromFile(std::string configFileName);

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
            return this->m_newLine;
        }

        /**
         * Verify if token value is reserved by another token name
         *
         */
        std::string updateTokenName(std::string tokenName, std::string tokenValue);
    private:

        // Define new line
        std::string m_newLine;

        // Token names ignored
        std::string m_ignorePrefix;
        std::string m_ignoreSuffix;
        std::set<std::string> m_ignoreInclude;
        std::set<std::string> m_ignoreExclude;

        // Token names classified as error tokens
        std::string m_errorPrefix;
        std::string m_errorSuffix;
        std::set<std::string> m_errorInclude;
        std::set<std::string> m_errorExclude;

        // Reserved token names
        std::map<std::string, std::map<std::string, std::string>> m_reservedTokens;
    };
}

#endif

#include <easycc/LexicalConfig.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string/predicate.hpp>

namespace ecc{

    // Define line separators
    const std::string LexicalConfig::LF = "LF";
    const std::string LexicalConfig::CR = "CR";
    const std::string LexicalConfig::CRLF = "CRLF";

    std::shared_ptr<LexicalConfig> LexicalConfig::buildConfigFromFile(std::string configFileName) {

        // Configuration JSON format
        const char* NEWLINE = "newline";
        const char* IGNORE = "ignore";
        const char* ERROR = "error";
        const char* RESERVED = "reserved";
        const char* PREFIX = "prefix";
        const char* SUFFIX = "suffix";
        const char* INCLUDE = "include";
        const char* EXCLUDE = "exclude";

        // Load file into string stream
        std::ifstream file(configFileName);
        std::stringstream  buffer;
        buffer << file.rdbuf();

        // Prepare a new LexicalConfig
        std::shared_ptr<LexicalConfig> config = std::make_shared<LexicalConfig>();

        // Parse json
        rapidjson::Document d;
        d.Parse(buffer.str().c_str());

        // Get the newline separator
        std::string configNewline = std::string(d[NEWLINE].GetString());
        if(configNewline != LF && configNewline != CR &&  configNewline != CRLF) {
            throw std::runtime_error("Configuration error! newline has to be: LF, CR or CRLF");
        }

        // Store newline
        config->m_newLine = configNewline;

        // Ignore object
        config->m_ignorePrefix = d[IGNORE][PREFIX].GetString();
        config->m_ignoreSuffix = d[IGNORE][SUFFIX].GetString();
        for(auto i=d[IGNORE][INCLUDE].Begin(); i!=d[IGNORE][INCLUDE].End(); i++) {
            config->m_ignoreInclude.insert((*i).GetString());
        }
        for(auto i=d[IGNORE][EXCLUDE].Begin(); i!=d[IGNORE][EXCLUDE].End(); i++) {
            config->m_ignoreExclude.insert((*i).GetString());
        }

        // Error object
        config->m_errorPrefix = d[ERROR][PREFIX].GetString();
        config->m_errorSuffix = d[ERROR][SUFFIX].GetString();
        for(auto i=d[ERROR][INCLUDE].Begin(); i!=d[ERROR][INCLUDE].End(); i++) {
            config->m_errorInclude.insert((*i).GetString());
        }
        for(auto i=d[ERROR][EXCLUDE].Begin(); i!=d[ERROR][EXCLUDE].End(); i++) {
            config->m_errorExclude.insert((*i).GetString());
        }

        // Reserved object
        for(auto i=d[RESERVED].MemberBegin(); i!=d[RESERVED].MemberEnd(); i++) {
            const char* tokenName = (*i).name.GetString();
            for(auto j=d[RESERVED][tokenName].MemberBegin(); j!= d[RESERVED][tokenName].MemberEnd(); j++) {
                config->m_reservedTokens[tokenName][j->name.GetString()] = j->value.GetString();
            }
        }
        return config;
    }

    bool LexicalConfig::mustIgnoreToken(std::string tokenName) {
        // If token name is in the exclude set
        if(this->m_ignoreExclude.find(tokenName) != this->m_ignoreExclude.end()) {
            return false;
        }

        // If token name is in the include set
        if(this->m_ignoreInclude.find(tokenName) != this->m_ignoreInclude.end()) {
            return true;
        }

        // If token name starts with the prefix
        if(m_ignorePrefix.size() > 0 && boost::algorithm::starts_with(tokenName, m_ignorePrefix)) {
            return true;
        }

        // If token name ends with the suffix
        return m_ignoreSuffix.size() > 0 && boost::algorithm::ends_with(tokenName, m_ignoreSuffix);
    }

    bool LexicalConfig::isErrorToken(std::string tokenName) {
        // If token name is in the exclude set
        if(this->m_errorExclude.find(tokenName) != this->m_errorExclude.end()) {
            return false;
        }

        // If token name is in the include set
        if(this->m_errorInclude.find(tokenName) != this->m_errorInclude.end()) {
            return true;
        }

        // If token name starts with the prefix
        if(m_errorPrefix.size() > 0 && boost::algorithm::starts_with(tokenName, m_errorPrefix)) {
            return true;
        }

        // If token name ends with the suffix
        return m_errorSuffix.size() > 0 && boost::algorithm::ends_with(tokenName, m_errorSuffix);
    }

    std::string LexicalConfig::updateTokenName(std::string tokenName, std::string tokenValue) {

        // If token name does not have any check entry
        if(this->m_reservedTokens.count(tokenName) == 0) {
            return tokenName;
        }

        // If token value exist in the map
        if(this->m_reservedTokens[tokenName].count(tokenValue) == 0) {
            return tokenName;
        }

        // Return new token name
        return this->m_reservedTokens[tokenName][tokenValue];
    }
}

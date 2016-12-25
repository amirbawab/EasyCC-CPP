#include "Config.h"
#include "../../rapidjson/document.h"
#include "../../rapidjson/writer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string/predicate.hpp>

namespace ecc{
    std::shared_ptr<Config> Config::buildConfig(std::string configFileName) {

        // Configuration JSON format
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

        // Prepare a new Config
        std::shared_ptr<Config> config = std::make_shared<Config>();

        // Parse json
        rapidjson::Document d;
        d.Parse(buffer.str().c_str());

        // Ignore object
        config->ignorePrefix = d[IGNORE][PREFIX].GetString();
        config->ignoreSuffix = d[IGNORE][SUFFIX].GetString();
        for(auto i=d[IGNORE][INCLUDE].Begin(); i!=d[IGNORE][INCLUDE].End(); i++) {
            config->ignoreInclude.insert((*i).GetString());
        }
        for(auto i=d[IGNORE][EXCLUDE].Begin(); i!=d[IGNORE][EXCLUDE].End(); i++) {
            config->ignoreExclude.insert((*i).GetString());
        }

        // Error object
        config->errorPrefix = d[ERROR][PREFIX].GetString();
        config->errorSuffix = d[ERROR][SUFFIX].GetString();
        for(auto i=d[ERROR][INCLUDE].Begin(); i!=d[ERROR][INCLUDE].End(); i++) {
            config->errorInclude.insert((*i).GetString());
        }
        for(auto i=d[ERROR][EXCLUDE].Begin(); i!=d[ERROR][EXCLUDE].End(); i++) {
            config->errorExclude.insert((*i).GetString());
        }

        // Reserved object
        for(auto i=d[RESERVED].MemberBegin(); i!=d[RESERVED].MemberEnd(); i++) {
            const char* tokenName = (*i).name.GetString();
            for(auto j=d[RESERVED][tokenName].MemberBegin(); j!= d[RESERVED][tokenName].MemberEnd(); j++) {
                config->reservedTokens[tokenName][j->name.GetString()] = j->value.GetString();
            }
        }
        return config;
    }

    bool Config::mustIgnoreToken(std::string tokenName) {
        // If token name is in the exclude set
        if(this->ignoreExclude.find(tokenName) != this->ignoreExclude.end()) {
            return false;
        }

        // If token name is in the include set
        if(this->ignoreInclude.find(tokenName) != this->ignoreInclude.end()) {
            return true;
        }

        // If token name starts with the prefix
        if(boost::algorithm::starts_with(tokenName, ignorePrefix)) {
            return true;
        }

        // If token name ends with the suffix
        return boost::algorithm::ends_with(tokenName, ignoreSuffix);
    }

    bool Config::isErrorToken(std::string tokenName) {
        // If token name is in the exclude set
        if(this->errorExclude.find(tokenName) != this->errorExclude.end()) {
            return false;
        }

        // If token name is in the include set
        if(this->errorInclude.find(tokenName) != this->errorInclude.end()) {
            return true;
        }

        // If token name starts with the prefix
        if(boost::algorithm::starts_with(tokenName, errorPrefix)) {
            return true;
        }

        // If token name ends with the suffix
        return boost::algorithm::ends_with(tokenName, errorSuffix);
    }

    std::string Config::updateTokenName(std::string tokenName, std::string tokenValue) {

        // If token name does not have any check entry
        if(this->reservedTokens.count(tokenName) == 0) {
            return tokenName;
        }

        // If token value exist in the map
        if(this->reservedTokens[tokenName].count(tokenValue) == 0) {
            return tokenName;
        }

        // Return new token name
        return this->reservedTokens[tokenName][tokenValue];
    }
}

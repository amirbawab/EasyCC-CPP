#include "Config.h"
#include "../../rapidjson/document.h"
#include "../../rapidjson/writer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

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
            config->ignoreInclude.push_back((*i).GetString());
        }
        for(auto i=d[IGNORE][EXCLUDE].Begin(); i!=d[IGNORE][EXCLUDE].End(); i++) {
            config->ignoreExclude.push_back((*i).GetString());
        }

        // Error object
        config->errorPrefix = d[ERROR][PREFIX].GetString();
        config->errorSuffix = d[ERROR][SUFFIX].GetString();
        for(auto i=d[ERROR][INCLUDE].Begin(); i!=d[ERROR][INCLUDE].End(); i++) {
            config->errorInclude.push_back((*i).GetString());
        }
        for(auto i=d[ERROR][EXCLUDE].Begin(); i!=d[ERROR][EXCLUDE].End(); i++) {
            config->errorExclude.push_back((*i).GetString());
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
}

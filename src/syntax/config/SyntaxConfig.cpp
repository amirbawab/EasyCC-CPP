#include <easycc/SyntaxConfig.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string/predicate.hpp>

namespace ecc{

    std::shared_ptr<SyntaxConfig> SyntaxConfig::buildConfigFromFile(std::string configFileName) {
        std::ifstream ifs(configFileName);
        rapidjson::IStreamWrapper isw(ifs);
        rapidjson::Document d;
        d.ParseStream(isw);
        return buildConfig(d);
    }

    std::shared_ptr<SyntaxConfig> SyntaxConfig::buildConfigFromString(std::string data) {
        rapidjson::Document d;
        d.Parse(data.c_str());
        return buildConfig(d);
    }

    std::shared_ptr<SyntaxConfig> SyntaxConfig::buildConfig(rapidjson::Document &d) {

        // Configuration JSON format
        const char* PARSING_PHASES = "parsing_phases";

        // Prepare a new SyntaxConfig
        std::shared_ptr<SyntaxConfig> config = std::make_shared<SyntaxConfig>();

        // Store the number parsing phases required
        config->m_parsingPhases = d[PARSING_PHASES].GetInt();

        return config;
    }
}

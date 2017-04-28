#include "SyntaxConfig.h"
#include "../../rapidjson/document.h"
#include "../../rapidjson/writer.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string/predicate.hpp>

namespace ecc{

    std::shared_ptr<SyntaxConfig> SyntaxConfig::buildConfig(std::string configFileName) {

        // Configuration JSON format
        const char* PARSING_PHASES = "parsing_phases";

        // Load file into string stream
        std::ifstream file(configFileName);
        std::stringstream  buffer;
        buffer << file.rdbuf();

        // Prepare a new SyntaxConfig
        std::shared_ptr<SyntaxConfig> config = std::make_shared<SyntaxConfig>();

        // Parse json
        rapidjson::Document d;
        d.Parse(buffer.str().c_str());

        // Store the number parsing phases required
        config->parsingPhases = d[PARSING_PHASES].GetInt();

        return config;
    }
}

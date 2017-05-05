#include <easycc/tools/ConvertResources.h>
#include <easycc/tools/Base64.h>
#include <getopt.h>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <fstream>
#include "rapidjson/filewritestream.h"
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(ecc_logger, src::logger_mt)
namespace ecc{
    void ConvertResources::initParams(int argc, char *argv[]) {

        struct option longOptions[] = {
                {"verbose", no_argument, 0, 'v'},
                {"template", required_argument, 0, 't'},
                {"output", required_argument, 0, 'o'},
                {"help", no_argument, 0, 'h'},
                {0, 0, 0, 0}
        };

        int optionIndex = 0;
        int c;
        while ((c = getopt_long(argc, argv, "hvo:t:", longOptions, &optionIndex)) != -1) {
            switch (c) {
                case 'v':
                    logging::add_console_log(
                            std::cout,
                            boost::log::keywords::format = "[%TimeStamp%]: %Message%"
                    );
                    logging::core::get()->set_logging_enabled(true);
                    break;
                case 'o':
                    m_outputFile = optarg;
                    break;
                case 't':
                    m_templateFile = optarg;
                    break;
                case 'h':
                default:
                    // Print by default
                    break;
            }
        }
    }

    void ConvertResources::printUsage() {
        std::cout
        << "Convert Resources - Convert a JSON file to a C++ header file" << std::endl
        << "Usage: convert-resources -t <template> -o <output> [OPTION]... [PATTERN=JSON_PATH]..." << std::endl
        << "\t-t, --template\t\tTemplate file containing patterns" << std::endl
        << "\t-o, --output\t\tOutput file" << std::endl
        << "\t-v, --verbose\t\tVerbose mode" << std::endl
        << "\t-h, --help\t\tDisplay this help message" << std::endl << std::endl;
    }

    int ConvertResources::run(int argc, char **argv) {
        initParams(argc, argv);

        // Init logs
        logging::add_common_attributes();
        logging::core::get()->set_logging_enabled(false);

        // Loop on additional arguments
        for(int i = optind; i < argc; ++i) {
            m_patternJson.push_back(argv[i]);
        }

        // Check if all the required arguments are passed
        if(m_patternJson.empty() || m_outputFile.empty() || m_templateFile.empty()) {
            return ConvertResources::RETURN_ERROR;
        }

        // Start converting
        return convert();
    }

    int ConvertResources::convert() {

        // Prepare output file
        std::ofstream oFile(m_outputFile);

        if (!oFile.is_open()) {
            std::cerr << "Cannot open outfile file: " << m_outputFile << std::endl;
            return RETURN_ERROR;
        } else {

            // Load template into string
            std::ifstream tFile(m_templateFile);

            if (!tFile.is_open()) {
                std::cerr << "Cannot open template file: " << m_templateFile << std::endl;
                return RETURN_ERROR;
            } else {

                // Prepare final content
                std::string generatedContent;

                // Store template into string
                std::stringstream buffer;
                buffer << tFile.rdbuf();

                // By default generated content looks like the template
                generatedContent = buffer.str();

                // Data copied into string stream, so closing template file
                tFile.close();

                for(std::string pair : m_patternJson) {

                    // Split the string
                    std::vector<std::string> patternJson;
                    boost::split(patternJson, pair, boost::is_any_of("="));

                    // Store pair in variables
                    std::string pattern = patternJson[0];
                    std::string jsonFile = patternJson[1];

                    // Load json
                    std::ifstream jFile(jsonFile);

                    if(!jFile.is_open()) {
                        std::cerr << "Cannot open JSON file: " << jsonFile << std::endl;
                        return RETURN_ERROR;
                    } else {

                        // Parse JSON file
                        rapidjson::IStreamWrapper isw(jFile);
                        rapidjson::Document d;
                        d.ParseStream(isw);

                        // Store minify JSON into string stream
                        std::stringstream jsonStringStream;
                        rapidjson::OStreamWrapper osw(jsonStringStream);
                        rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
                        d.Accept(writer);
                        std::string jsonString = jsonStringStream.str();

                        // Encode JSON
                        std::string jsonB64 = encode64(jsonString);

                        // Replace patterns with the JSON file content
                        boost::replace_all(generatedContent, pattern, jsonB64);

                        jFile.close();
                    }
                }

                // Store new content into the output file
                oFile << generatedContent;
            }
            oFile.close();
        }
        return RETURN_SUCCESS;
    }
}

int main(int argc, char *argv[]) {
    ecc::ConvertResources convertResources;
    return convertResources.run(argc, argv);
}
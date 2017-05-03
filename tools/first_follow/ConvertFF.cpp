#include <easycc/tools/ConvertFF.h>
#include <iostream>
#include <getopt.h>
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(ecc_logger, src::logger_mt)

namespace ecc{

    void ConvertFF::init(int argc, char*argv[]) {

        // Init logs
        logging::add_common_attributes();
        logging::core::get()->set_logging_enabled(false);

        // Initialize parameters
        initParams(argc, argv);
    }

    void ConvertFF::generateFirstSet(std::stringstream &stream, Grammar &grammar) {

        for(std::string nonTerminal : grammar.getNonTerminals()) {
            stream << "            "
            << "this->m_firstSet[\""<< nonTerminal
            << "\"] = std::make_shared<std::set<std::string>>();" << std::endl;

            for(std::string token : *grammar.getFirstSet(nonTerminal)) {
                stream << "            "
                << "this->m_firstSet[\"" << nonTerminal
                << "\"]->insert(\"" << token << "\");" << std::endl;
            }
            stream << std::endl;
        }
    }

    void ConvertFF::generateFollowSet(std::stringstream &stream, Grammar &grammar) {
        for(std::string nonTerminal : grammar.getNonTerminals()) {
            stream << "            "
            << "this->m_followSet[\""<< nonTerminal
            << "\"] = std::make_shared<std::set<std::string>>();" << std::endl;

            for(std::string token : *grammar.getFollowSet(nonTerminal)) {
                stream << "            "
                << "this->m_followSet[\"" << nonTerminal
                << "\"]->insert(\"" << token << "\");" << std::endl;
            }
            stream << std::endl;
        }
    }

    void ConvertFF::generateParseTable(std::stringstream &stream, Grammar &grammar) {
        for(std::string nonTerminal : grammar.getNonTerminals()) {

            // Create set for the non-terminal
            stream << "            "
            << "this->m_parseTableMap[\"" << nonTerminal
            << "\"] = std::make_shared<std::map<std::string,"
            << "std::shared_ptr<std::vector<std::string>>>>();" << std::endl;

            // Populate the created map
            for(const auto &parseEntry : *grammar.getParseMap(nonTerminal)) {
                stream << "            "
                << "*(this->m_parseTableMap[\"" << nonTerminal
                << "\"])[\"" << parseEntry.first << "\"] = "
                << "std::make_shared<std::vector<std::string>>({";

                for(int i=0; i < parseEntry.second->size(); i++) {
                    if(i == 0) {
                        stream << "\"" << (*parseEntry.second)[i] << "\"";
                    } else {
                        stream << ",\"" << (*parseEntry.second)[i] << "\"";
                    }
                }
                stream << "});" << std::endl;
            }
            stream << std::endl;
        }
    }

    void ConvertFF::convert(std::string fileName) {

        // Process the grammar file
        Grammar grammar(fileName);

        // Prepare output file
        std::ofstream file(m_outputFile);

        // Check if can open file
        if(!file.is_open()) {
            std::cerr << "Cannot open file: " << m_outputFile << std::endl;
        } else {

            // Load template into string
            std::ifstream inputFile(m_inputFile);

            if(!inputFile.is_open()) {
                std::cerr << "Cannot open file: " << m_inputFile << std::endl;
            } else {

                // Store template into string
                std::stringstream buffer;
                buffer << inputFile.rdbuf();
                std::string content = buffer.str();

                // Close file because its content is copied
                inputFile.close();

                std::stringstream firstSetStream;
                std::stringstream followSetStream;
                std::stringstream parseTableStream;

                generateFirstSet(firstSetStream, grammar);
                generateFollowSet(followSetStream, grammar);
                generateParseTable(parseTableStream, grammar);

                // Replace patterns
                boost::replace_all(content, FIRST_SET_PATTERN, firstSetStream.str());
                boost::replace_all(content, FOLLOW_SET_PATTERN, followSetStream.str());
                boost::replace_all(content, PARSE_TABLE_PATTERN, parseTableStream.str());

                file << content;
            }
            file.close();
        }
    }

    void ConvertFF::printUsage() {
        std::cout
        << "First Follow - Convert syntax grammar into a C++ header file" << std::endl
        << "Usage: firstfollow -t template.h -o output.h [OPTION]... FILE" << std::endl
        << "\t-t, --template\t\tInput template file" << std::endl
        << "\t-o, --output\t\tOutput file" << std::endl
        << "\t-v, --verbose\t\tVerbose mode" << std::endl
        << "\t-h, --help\t\tDisplay this help message" << std::endl << std::endl
        << "Replaced patterns:" << std::endl
        << "\t" << FIRST_SET_PATTERN << std::endl
        << "\t" << FOLLOW_SET_PATTERN << std::endl
        << "\t" << PARSE_TABLE_PATTERN << std::endl;
    }

    void ConvertFF::initParams(int argc, char *argv[]) {

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
                    m_inputFile = optarg;
                    break;
                case 'h':
                default:
                    // Print by default
                    break;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    ecc::ConvertFF convertFF;
    convertFF.init(argc, argv);

    // Check if all the required arguments are passed
    int fileIndex = optind;
    if(fileIndex >= argc || convertFF.getOutputFile().empty() || convertFF.getInputFile().empty()) {
        convertFF.printUsage();
        return 1;
    }

    // Convert file
    convertFF.convert(argv[fileIndex]);

    return 0;
}
#include <easycc/EasyCCDev.h>
#include <iostream>
#include <getopt.h>

#include <boost/move/utility_core.hpp>
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

    int EasyCC::init(int argc, char*argv[]) {

        // Configure the logger
        initLogs();

        // Initialize parameters
        initParams(argc, argv);

        // Get files
        fetchInputFiles(argc, argv);

        // Check if required arguments are set
        if(!validArguments()) {
            printUsage();
            return ERR_CODE_PARAMS;
        }

        // Create analyzers
        m_lexical = std::make_shared<Lexical>();
        m_lexical->buildFromFiles(m_lexicalStateMachineFile, m_lexicalConfigFile, m_lexicalErrorsFile);

        m_syntax = std::make_shared<Syntax>();
        m_syntax->buildFromFiles(m_syntaxGrammarFile, m_syntaxConfigFile, m_syntaxErrorsFile);

        // Set semantic action after m_syntax is created
        setSemanticAction();
    }

    void EasyCC::initLogs() {
        logging::add_common_attributes();
        logging::core::get()->set_logging_enabled(false);
    }

    void EasyCC::printUsage() {
        std::cout
        << "EasyCC C++ - An easy compiler compiler program" << std::endl
        << "Usage: easycc [OPTION]... [FILE]..." << std::endl
        << "\t-s, --lexical_state_machine\t[required] Lexical state machine file" << std::endl
        << "\t-c, --lexical_config\t\t[required] Lexical configuration file" << std::endl
        << "\t-e, --lexical_errors\t\t[required] Lexical errors file" << std::endl
        << "\t-g, --syntax_grammar\t\t[required] Syntax grammar file" << std::endl
        << "\t-C, --syntax_config\t\t[required] Syntax configuration file" << std::endl
        << "\t-E, --syntax_errors\t\t[required] Syntax errors file" << std::endl
        << "\t-o, --output\t\t\tOutput file" << std::endl
        << "\t-v, --verbose\t\t\tVerbose mode" << std::endl
        << "\t-h, --help\t\t\tDisplay this help message" << std::endl;
    }

    void EasyCC::initParams(int argc, char *argv[]) {

        struct option longOptions[] = {

                // Lexical params
                {"lexical_state_machine", required_argument, 0, 's'},
                {"lexical_config", required_argument, 0, 'c'},
                {"lexical_errors", required_argument, 0, 'e'},

                // Syntax params
                {"syntax_grammar", required_argument, 0, 'g'},
                {"syntax_config", required_argument, 0, 'C'},
                {"syntax_errors", required_argument, 0, 'E'},

                // Other
                {"output", required_argument, 0, 'o'},
                {"verbose", no_argument, 0, 'v'},
                {"help", no_argument, 0, 'h'},
                {0, 0, 0, 0}
        };

        int optionIndex = 0;
        int c;
        while ((c = getopt_long(argc, argv, "hvs:c:e:g:C:E:o:", longOptions, &optionIndex)) != -1) {
            switch (c) {
                case 's':
                    m_lexicalStateMachineFile = optarg;
                    break;
                case 'c':
                    m_lexicalConfigFile = optarg;
                    break;
                case 'e':
                    m_lexicalErrorsFile = optarg;
                    break;
                case 'g':
                    m_syntaxGrammarFile = optarg;
                    break;
                case 'C':
                    m_syntaxConfigFile = optarg;
                    break;
                case 'E':
                    m_syntaxErrorsFile = optarg;
                    break;
                case 'o':
                    m_outputFile = optarg;
                    break;
                case 'v':
                    logging::add_console_log(
                            std::cout,
                            boost::log::keywords::format = "[%TimeStamp%]: %Message%"
                    );
                    logging::core::get()->set_logging_enabled(true);
                    break;
                case 'h':
                default:
                    // Print by default
                    break;
            }
        }
    }

    bool EasyCC::validArguments() {
        return  !m_lexicalStateMachineFile.empty() &&
                !m_lexicalConfigFile.empty() &&
                !m_lexicalErrorsFile.empty() &&
                !m_syntaxGrammarFile.empty() &&
                !m_syntaxConfigFile.empty() &&
                !m_syntaxErrorsFile.empty() &&
                !m_inputFiles.empty();
    }

    void EasyCC::fetchInputFiles(const int argc, char *argv[]) {
        for(int i = optind; i < argc; ++i) {
            m_inputFiles.push_back(argv[i]);
        }
    }
}


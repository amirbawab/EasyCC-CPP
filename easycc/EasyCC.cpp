#include "EasyCC.h"
#include "../lexical/Lexical.h"
#include "../syntax/Syntax.h"
#include <iostream>
#include <getopt.h>
#include <vector>
#include <string>

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
        lexical = std::make_shared<Lexical>(lexicalStateMachineFile, lexicalConfigFile, lexicalErrorsFile);
        syntax = std::make_shared<Syntax>(syntaxGrammarFile, syntaxConfigFile, syntaxErrorsFile);

        // Handle syntax events
        syntax->setSemanticAction([&](std::string semanticAction, int phase,
                                      std::vector<std::shared_ptr<LexicalToken>> &lexicalTokensParam,
                                      int index, bool stable) -> void {
            if(semanticActionMap.find(semanticAction) == semanticActionMap.end()) {
                BOOST_LOG(ecc_logger::get()) << "Error: Cannot find a handler for the semantic action: " << semanticAction;
            } else {
                semanticActionMap[semanticAction](phase, lexicalTokensParam, index, stable);
            }
        });
    }

    void EasyCC::registerSemanticAction(std::string semanticAction, std::function<void
            (int, std::vector<std::shared_ptr<LexicalToken>>&, int, bool)> semanticActionFunction) {
        if(semanticActionMap.find(semanticAction) == semanticActionMap.end()) {
            semanticActionMap[semanticAction] = semanticActionFunction;
            BOOST_LOG(ecc_logger::get()) << "Handler registered for the semantic action: " << semanticAction;
        } else {
            BOOST_LOG(ecc_logger::get()) << "Error: Handler already registered for the semantic action: "
                                         << semanticAction;
        }
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
                    lexicalStateMachineFile = optarg;
                    break;
                case 'c':
                    lexicalConfigFile = optarg;
                    break;
                case 'e':
                    lexicalErrorsFile = optarg;
                    break;
                case 'g':
                    syntaxGrammarFile = optarg;
                    break;
                case 'C':
                    syntaxConfigFile = optarg;
                    break;
                case 'E':
                    syntaxErrorsFile = optarg;
                    break;
                case 'o':
                    outputFile = optarg;
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
        return  !lexicalStateMachineFile.empty() &&
                !lexicalConfigFile.empty() &&
                !lexicalErrorsFile.empty() &&
                !syntaxGrammarFile.empty() &&
                !syntaxConfigFile.empty() &&
                !syntaxErrorsFile.empty() &&
                !inputFiles.empty();
    }

    void EasyCC::fetchInputFiles(const int argc, char *argv[]) {
        for(int i = optind; i < argc; ++i) {
            inputFiles.push_back(argv[i]);
        }
    }

    int EasyCC::compile() {

        for(std::string inputFile : inputFiles) {

            // Start lexical analyzer
            std::vector<std::shared_ptr<LexicalToken>> lexicalTokens;
            std::vector<std::string> lexicalErrorMessages;
            lexical->generateLexicalTokens(inputFile, lexicalTokens, lexicalErrorMessages);

            // Logging
            for(auto message : lexicalErrorMessages)
                std::cerr << message << std::endl;
            if(lexicalErrorMessages.size() != 0) {
                // A lexical error exist, exit
                std::cerr << "Exiting program with code " << ERR_CODE_LEXICAL << std::endl;
                return ERR_CODE_LEXICAL;
            }

            // Start syntax analyzer
            std::vector<std::string> syntaxErrorMessages;
            syntax->parseTokens(lexicalTokens, syntaxErrorMessages);

            // Logging syntax phase
            for(auto message : syntaxErrorMessages)
                std::cerr << message << std::endl;
            if(syntaxErrorMessages.size() != 0) {
                // A syntax error exist, exit
                std::cerr << "Exiting program with code " << ERR_CODE_SYNTAX << std::endl;
                return ERR_CODE_SYNTAX;
            }
        }

        return OK_CODE;
    }
}


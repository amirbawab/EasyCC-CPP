#include "../lexical/Lexical.h"
#include "../syntax/Syntax.h"
#include "../semantic/Semantic.h"
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
#include <boost/log/core/core.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;

using namespace std;
using namespace ecc;

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(ecc_logger, src::logger_mt)

// Configuration files
std::string lexicalStateMachineFile;
std::string lexicalConfigFile;
std::string lexicalErrorsFile;
std::string syntaxGrammarFile;
std::string syntaxConfigFile;
std::string syntaxErrorsFile;
std::string outputFile;
std::vector<std::string> inputFiles;

// Error codes
const int ERR_CODE_PARAMS = 1;
const int ERR_CODE_LEXICAL = 2;
const int ERR_CODE_SYNTAX = 3;

void init_log() {
    logging::add_common_attributes();
    logging::core::get()->set_logging_enabled(false);
}

void printUsage() {
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
    << "\t-h, --help\t\t\tDisplay this help message" << endl;
}

void initParams(int argc, char *argv[]) {

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

bool validArguments() {
    return  !lexicalStateMachineFile.empty() &&
            !lexicalConfigFile.empty() &&
            !lexicalErrorsFile.empty() &&
            !syntaxGrammarFile.empty() &&
            !syntaxConfigFile.empty() &&
            !syntaxErrorsFile.empty() &&
            !inputFiles.empty();
}

void fetchInputFiles(const int &argc, char *argv[]) {
    for(int i = optind; i < argc; ++i) {
        inputFiles.push_back(argv[i]);
    }
}

int main(int argc, char *argv[]) {

    // Configure the logger
    init_log();

    // Initialize parameters
    initParams(argc, argv);

    // Get files
    fetchInputFiles(argc, argv);

    // Check if required arguments are set
    if(!validArguments()) {
        printUsage();
        return ERR_CODE_PARAMS;
    }

    // Lexical analysis phase
    Lexical lexical(lexicalStateMachineFile, lexicalConfigFile, lexicalErrorsFile);

	std::vector<std::shared_ptr<LexicalToken>> lexicalTokens;
	std::vector<std::string> lexicalErrorMessages;
	lexical.generateLexicalTokens(inputFiles[0], lexicalTokens, lexicalErrorMessages);

    // Logging
	for(auto message : lexicalErrorMessages)
		cerr << message << endl;
    if(lexicalErrorMessages.size() != 0) {
        // A lexical error exist, exit
        cerr << "Exiting program with code " << ERR_CODE_LEXICAL << endl;
        return ERR_CODE_LEXICAL;
    }

    // Syntax analysis phase
    std::vector<std::string> syntaxErrorMessages;
    Syntax syntax(syntaxGrammarFile, syntaxConfigFile, syntaxErrorsFile);

    // Prepare semantic analysis
    Semantic semantic;

    // Handle syntax events
    syntax.setSemanticAction([&](std::string semanticAction, int phase,
                                std::vector<std::shared_ptr<LexicalToken>> &lexicalTokensParam,
                                 int index, bool stable) -> void {
        semantic.handle(semanticAction, phase, lexicalTokensParam, index, stable);
    });

    // Parse the generated lexical tokens
    syntax.parseTokens(lexicalTokens, syntaxErrorMessages);

    // Logging syntax phase
    for(auto message : syntaxErrorMessages)
        cerr << message << endl;
    if(syntaxErrorMessages.size() != 0) {
        // A syntax error exist, exit
        cerr << "Exiting program with code " << ERR_CODE_SYNTAX << endl;
        return ERR_CODE_SYNTAX;
    }

	return 0;
}

#include "../lexical/Lexical.h"
#include "../syntax/Syntax.h"
#include "../semantic/Semantic.h"
#include <iostream>
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

using namespace std;
using namespace ecc;

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(ecc_logger, src::logger_mt)

void init_log() {
    logging::add_console_log(
            std::cout,
            boost::log::keywords::format = "[%TimeStamp%]: %Message%"
    );
    logging::add_file_log(
            keywords::file_name = "logs_%N.log",
            keywords::rotation_size = 10 * 1024 * 1024,
            keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
            keywords::auto_flush = true,
            keywords::format = "[%TimeStamp%]: %Message%"
    );
    logging::add_common_attributes();
}

int main() {

    // Configure the logger
    init_log();

    // Lexical analysis phase
    Lexical lexical(
			"resources/src/lexical_state_machine.json",
			"resources/src/lexical_config.json",
            "resources/src/lexical_errors.json");

	std::vector<std::shared_ptr<LexicalToken>> lexicalTokens;
	std::vector<std::string> lexicalErrorMessages;
	lexical.generateLexicalTokens("resources/src/input.txt", lexicalTokens, lexicalErrorMessages);

    // Logging
	for(auto token : lexicalTokens)
		cout << *token << endl;
	for(auto message : lexicalErrorMessages)
		cerr << message << endl;
    if(lexicalErrorMessages.size() != 0) {
        // A lexical error exist, exit
        cerr << "Exiting program with code 1" << endl;
        return 1;
    }

    // Syntax analysis phase
    std::vector<std::string> syntaxErrorMessages;
    Syntax syntax(
            "resources/src/syntax_grammar.txt",
            "resources/src/syntax_errors.json");

    // Prepare semantic analysis
    Semantic semantic;

    // Handle syntax events
    syntax.setSemanticAction([&](std::string semanticAction, int phase,
                                std::vector<std::shared_ptr<LexicalToken>> &lexicalTokensParam, int index) -> void {
        semantic.handle(semanticAction, phase, lexicalTokensParam, index);
    });

    // Parse the generated lexical tokens
    syntax.parseTokens(lexicalTokens, syntaxErrorMessages);

    // Logging syntax phase
    for(auto message : syntaxErrorMessages)
        cerr << message << endl;
    if(syntaxErrorMessages.size() != 0) {
        // A syntax error exist, exit
        cerr << "Exiting program with code 2" << endl;
        return 2;
    }

	return 0;
}

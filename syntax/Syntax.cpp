#include "Syntax.h"

#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
namespace ecc{

    namespace src = boost::log::sources;
    BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(ecc_logger, src::logger_mt)

    Syntax::Syntax(std::string grammarFile) {

        // Build the grammar form file
        grammar = std::make_shared<Grammar>(grammarFile);
    }

    void Syntax::parseTokens(std::vector<std::shared_ptr<LexicalToken>> &lexicalTokens) {

        BOOST_LOG(ecc_logger::get()) << "Started parsing the lexical tokens ...";
    }
}

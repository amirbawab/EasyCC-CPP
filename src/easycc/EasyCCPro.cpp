#include <easycc/EasyCCPro.h>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <easycc/tools/StaticResources.h>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;

BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(ecc_logger, src::logger_mt)

namespace ecc{
    EasyCC::EasyCC() {

        // Disable logs
        logging::core::get()->set_logging_enabled(false);

        // Create analyzers
        m_lexical = std::make_shared<Lexical>();
        m_lexical->buildFromStrings(lexicalStateMachineJSON, lexicalConfigJSON, lexicalErrorsJSON);

        m_syntax = std::make_shared<Syntax>();
        m_syntax->buildFromStrings(syntaxGrammarJSON, syntaxConfigJSON, syntaxErrorsJSON);

        // Set semantic action after m_syntax is created
        setSemanticAction();
    }
}


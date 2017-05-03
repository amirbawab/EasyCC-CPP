#include <easycc/IEasyCC.h>
#include <iostream>

#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
namespace src = boost::log::sources;
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(ecc_logger, src::logger_mt)

namespace ecc {

    void IEasyCC::registerSemanticAction(std::string semanticAction, std::function<void
            (int, std::vector<std::shared_ptr<LexicalToken>>&, int, bool)> semanticActionFunction) {
        if(m_semanticActionMap.find(semanticAction) == m_semanticActionMap.end()) {
            m_semanticActionMap[semanticAction] = semanticActionFunction;
        } else {
            std::cerr << "Error: Handler already registered for the semantic action: "
            << semanticAction;
        }
    }

    void IEasyCC::setSemanticAction() {
        m_syntax->setSemanticAction([&](std::string semanticAction, int phase,
                                        std::vector<std::shared_ptr<LexicalToken>> &lexicalTokensParam,
                                        int index, bool stable) -> void {
            if(m_semanticActionMap.find(semanticAction) == m_semanticActionMap.end()) {
                BOOST_LOG(ecc_logger::get()) << "Error: Cannot find a handler for the semantic action: " << semanticAction;
            } else {
                m_semanticActionMap[semanticAction](phase, lexicalTokensParam, index, stable);
            }
        });
    }

    int IEasyCC::compile(std::string fileName) {

        // Start lexical analyzer
        std::vector<std::shared_ptr<LexicalToken>> lexicalTokens;
        std::vector<std::string> lexicalErrorMessages;
        m_lexical->generateLexicalTokens(fileName, lexicalTokens, lexicalErrorMessages);

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
        m_syntax->parseTokens(lexicalTokens, syntaxErrorMessages);

        // Logging syntax phase
        for(auto message : syntaxErrorMessages)
            std::cerr << message << std::endl;
        if(syntaxErrorMessages.size() != 0) {
            // A syntax error exist, exit
            std::cerr << "Exiting program with code " << ERR_CODE_SYNTAX << std::endl;
            return ERR_CODE_SYNTAX;
        }

        return OK_CODE;
    }
}

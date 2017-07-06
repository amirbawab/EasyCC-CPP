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
                BOOST_LOG(ecc_logger::get()) << "Calling handler for the semantic action: " << semanticAction;
                m_semanticActionMap[semanticAction](phase, lexicalTokensParam, index, stable);
            }
        });
    }

    void IEasyCC::setSilentSyntaxErrorMessages(bool silent) {
        this->m_syntax->setSilentSyntaxErrorMessages(silent);
    }

    void IEasyCC::setSilentSemanticEvents(bool silent) {
        this->m_syntax->setSilentSemanticAction(silent);
    }

    void IEasyCC::setOnSyntaxError(std::function<void()> onSyntaxError) {
        this->m_syntax->setOnSyntaxError(onSyntaxError);
    }

    void IEasyCC::setParsingPhase(int phase) {
        this->m_syntax->setParsingPhase(phase);
    }

    int IEasyCC::compile(std::string fileName) {

        // Prepare lexical analyzer vector
        std::vector<std::shared_ptr<LexicalToken>> lexicalTokens;

        // If lexical tokens were generated for that file use them, otherwise
        // generate them. If generating them did not succeed, return error code
        if(m_lexicalTokensMap.find(fileName) != m_lexicalTokensMap.end()) {
            lexicalTokens = m_lexicalTokensMap[fileName];
        } else if(m_lexical->generateLexicalTokens(fileName, lexicalTokens)) {
            // TODO Add same file detector (e.g. "myFile.bc" and "../cur_dir/myFile.bc" are the same files)
            m_lexicalTokensMap[fileName] = lexicalTokens;
        } else {
            return ERR_CODE_LEXICAL;
        }

        // Start syntax analyzer
        if(!m_syntax->parseTokens(fileName, lexicalTokens)) {
            return ERR_CODE_SYNTAX;
        }

        return OK_CODE;
    }
}

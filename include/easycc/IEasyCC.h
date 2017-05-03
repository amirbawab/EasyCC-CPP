#ifndef EASYCC_IEASYCC_H
#define EASYCC_IEASYCC_H

#include <easycc/Lexical.h>
#include <easycc/Syntax.h>
#include <memory>
#include <vector>
#include <string>

namespace ecc{
    class IEasyCC {
    protected:
        std::shared_ptr<Lexical> m_lexical;
        std::shared_ptr<Syntax> m_syntax;
        void setSemanticAction();
    private:
        std::map<std::string, std::function<void
                (int, std::vector<std::shared_ptr<LexicalToken>>&, int, bool)>> m_semanticActionMap;
    public:
        // Return codes
        static const int OK_CODE = 0;
        static const int ERR_CODE_PARAMS = 1;
        static const int ERR_CODE_LEXICAL = 2;
        static const int ERR_CODE_SYNTAX = 3;

        virtual int compile(std::string fileName);
        virtual void registerSemanticAction(std::string semanticAction, std::function<void
            (int, std::vector<std::shared_ptr<LexicalToken>>&, int, bool)> semanticActionFunction);
    };
}

#endif

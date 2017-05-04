#ifndef EASYCC_CONVERTSYNTAXMESSAGES_H
#define EASYCC_CONVERTSYNTAXMESSAGES_H

#include <easycc/tools/Convertible.h>

namespace ecc {
    class ConvertSyntaxMessages : public Convertible {
    public:
        void convert(std::string fileName);
    };
}

#endif

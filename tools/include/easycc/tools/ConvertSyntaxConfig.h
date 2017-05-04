#ifndef EASYCC_CONVERTSYNTAXCONFIG_H
#define EASYCC_CONVERTSYNTAXCONFIG_H

#include <easycc/tools/Convertible.h>

namespace ecc {
    class ConvertSyntaxConfig : public Convertible {
    public:
        void convert(std::string fileName);
    };
}

#endif

#include "LexicalToken.h"

namespace ecc{
    // End of file symbol is injected at the end of the input
    // This value must match the value of END_OF_STACK in the syntax analysis
    const std::string LexicalToken::END_OF_FILE = "$";
}
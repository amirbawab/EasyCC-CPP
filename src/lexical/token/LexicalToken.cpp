#include <easycc/LexicalToken.h>
#include <sstream>

namespace ecc{
    // End of file symbol is injected at the end of the input
    // This value must match the value of END_OF_STACK in the syntax analysis
    const std::string LexicalToken::END_OF_FILE = "$";

    std::string LexicalToken::getString() {
        std::ostringstream ss;
        ss << "<" << m_name << ",l:" << m_line << ",c:" << m_column << ">";
        return ss.str();
    }
}

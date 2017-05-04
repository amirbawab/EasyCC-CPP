#include <easycc/tools/ConvertSyntaxMessages.h>
#include <easycc/SyntaxMessages.h>
#include <boost/algorithm/string.hpp>
#include <sstream>

namespace ecc {
    const std::string DEFAULT_MESSAGE_PATTERN = "/*DEFAULT_MESSAGE*/";
    const std::string ERROR_MESSAGES_PATTERN = "/*ERROR_MESSAGES*/";

    void ConvertSyntaxMessages::convert(std::string fileName) {
        std::shared_ptr<SyntaxMessages> syntaxMessages = SyntaxMessages::loadMessages(fileName);

        // Write default messages
        std::stringstream stream;
        stream << "this->m_parsingPhases = \"" << syntaxMessages->getDefaultMessage()<< "\";" << std::endl;
        boost::replace_all(m_newContent, DEFAULT_MESSAGE_PATTERN, stream.str());
    }
}

int main(int argc, char *argv[]) {
    ecc::ConvertSyntaxMessages convertSyntaxMessages;
    if(convertSyntaxMessages.init(argc,argv) != ecc::Convertible::RETURN_SUCCESS) {
        convertSyntaxMessages.printUsage(
                "Syntax Messages - Convert syntax messages into a C++ file\n"
                        "Usage: convert-syntax-msg -t template.h -o output.h -i input [OPTION]...",
                "Replaced patterns:"
                        "\n\t" + ecc::DEFAULT_MESSAGE_PATTERN +
                        "\n\t" + ecc::ERROR_MESSAGES_PATTERN);
    }
    return convertSyntaxMessages.run();
}

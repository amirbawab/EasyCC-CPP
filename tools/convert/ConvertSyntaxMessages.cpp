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
        std::stringstream defaultMessageStream;
        defaultMessageStream << "        "
                "this->m_defaultMessage = \"" <<
                syntaxMessages->getDefaultMessage() << "\";" << std::endl;
        boost::replace_all(m_newContent, DEFAULT_MESSAGE_PATTERN, defaultMessageStream.str());

        // Write all error messages
        std::stringstream errorMessageStream;
        auto errorMessages = syntaxMessages->getErrorMessages();
        for(auto nonTerminalMap : errorMessages) {
            for(auto terminalMap : nonTerminalMap.second) {
                errorMessageStream << "        "
                        "this->m_errorMessages[\"" << nonTerminalMap.first << "\"][\""
                << terminalMap.first << "\"] = \"" << terminalMap.second << "\";" << std::endl;
            }
        }
        boost::replace_all(m_newContent, ERROR_MESSAGES_PATTERN, errorMessageStream.str());
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

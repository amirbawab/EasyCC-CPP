#include <easycc/tools/ConvertSyntaxConfig.h>
#include <easycc/SyntaxConfig.h>
#include <boost/algorithm/string.hpp>
#include <sstream>

namespace ecc{
    const std::string PARSING_PHASE_PATTERN = "/*PARSING_PHASE*/";

    void ConvertSyntaxConfig::convert(std::string fileName) {
        std::shared_ptr<SyntaxConfig> syntaxConfig = SyntaxConfig::buildConfig(fileName);

        // Write parsing phase
        std::stringstream stream;
        stream << "this->m_parsingPhases = " << syntaxConfig->getParsingPhases()<< ";" << std::endl;
        boost::replace_all(m_newContent, PARSING_PHASE_PATTERN, stream.str());
    }
}
int main(int argc, char *argv[]) {
    ecc::ConvertSyntaxConfig convertSyntaxConfig;
    if(convertSyntaxConfig.init(argc,argv) != ecc::Convertible::RETURN_SUCCESS) {
        convertSyntaxConfig.printUsage(
                "Syntax Configuration - Convert syntax configuration into a C++ file\n"
                        "Usage: convert-syntax-config -t template.h -o output.h -i input [OPTION]...",
                "Replaced patterns:"
                "\n\t" + ecc::PARSING_PHASE_PATTERN);
    }
    return convertSyntaxConfig.run();
}

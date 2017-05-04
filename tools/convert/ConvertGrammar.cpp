#include <easycc/tools/ConvertGrammar.h>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <sstream>

namespace ecc{

    const std::string FIRST_SET_PATTERN = "/*FIRST_SET*/";
    const std::string FOLLOW_SET_PATTERN = "/*FOLLOW_SET*/";
    const std::string PARSE_TABLE_PATTERN = "/*PARSE_TABLE*/";

    void ConvertGrammar::generateFirstSet(std::stringstream &stream, Grammar &grammar) {

        for(std::string nonTerminal : grammar.getNonTerminals()) {
            stream << "            "
            << "this->m_firstSet[\""<< nonTerminal
            << "\"] = std::make_shared<std::set<std::string>>();" << std::endl;

            for(std::string token : *grammar.getFirstSet(nonTerminal)) {
                stream << "            "
                << "this->m_firstSet[\"" << nonTerminal
                << "\"]->insert(\"" << token << "\");" << std::endl;
            }
            stream << std::endl;
        }
    }

    void ConvertGrammar::generateFollowSet(std::stringstream &stream, Grammar &grammar) {
        for(std::string nonTerminal : grammar.getNonTerminals()) {
            stream << "            "
            << "this->m_followSet[\""<< nonTerminal
            << "\"] = std::make_shared<std::set<std::string>>();" << std::endl;

            for(std::string token : *grammar.getFollowSet(nonTerminal)) {
                stream << "            "
                << "this->m_followSet[\"" << nonTerminal
                << "\"]->insert(\"" << token << "\");" << std::endl;
            }
            stream << std::endl;
        }
    }

    void ConvertGrammar::generateParseTable(std::stringstream &stream, Grammar &grammar) {
        for(std::string nonTerminal : grammar.getNonTerminals()) {

            // Create set for the non-terminal
            stream << "            "
            << "this->m_parseTableMap[\"" << nonTerminal
            << "\"] = std::make_shared<std::map<std::string,"
            << "std::shared_ptr<std::vector<std::string>>>>();" << std::endl;

            // Populate the created map
            for(const auto &parseEntry : *grammar.getParseMap(nonTerminal)) {
                stream << "            "
                << "(*this->m_parseTableMap[\"" << nonTerminal
                << "\"])[\"" << parseEntry.first << "\"] = "
                << "std::make_shared<std::vector<std::string>>();"
                << std::endl;


                for(const std::string &token : *parseEntry.second) {
                    stream << "            "
                    << "(*this->m_parseTableMap[\"" << nonTerminal
                    << "\"])[\"" << parseEntry.first << "\"]->push_back(\"" << token
                    << "\");" << std::endl;
                }
            }
            stream << std::endl;
        }
    }

    void ConvertGrammar::convert(std::string fileName) {

        // Process the grammar file
        Grammar grammar(fileName);

        std::stringstream firstSetStream;
        std::stringstream followSetStream;
        std::stringstream parseTableStream;

        generateFirstSet(firstSetStream, grammar);
        generateFollowSet(followSetStream, grammar);
        generateParseTable(parseTableStream, grammar);

        // Replace patterns
        boost::replace_all(m_newContent, FIRST_SET_PATTERN, firstSetStream.str());
        boost::replace_all(m_newContent, FOLLOW_SET_PATTERN, followSetStream.str());
        boost::replace_all(m_newContent, PARSE_TABLE_PATTERN, parseTableStream.str());
    }
}

int main(int argc, char *argv[]) {
    ecc::ConvertGrammar convertGrammar;
    if(convertGrammar.init(argc,argv) != ecc::Convertible::RETURN_SUCCESS) {
        convertGrammar.printUsage(
                "Syntax Grammar - Convert syntax grammar into a C++ file\n"
                        "Usage: convert-grammar -t template.h -o output.h -i input [OPTION]...",
                "Replaced patterns:"
                        "\n\t" + ecc::FIRST_SET_PATTERN +
                        "\n\t" + ecc::FOLLOW_SET_PATTERN +
                        "\n\t" + ecc::PARSE_TABLE_PATTERN );
    }
    return convertGrammar.run();
}
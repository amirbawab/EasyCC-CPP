#ifndef EASYCC_CPP_SYNTAXMESSAGES_H
#define EASYCC_CPP_SYNTAXMESSAGES_H

#include <map>
#include <string>
#include <memory>
#include <rapidjson/document.h>

namespace ecc{
    class SyntaxMessages {
    public:

        /**
         * Load messages from file
         * @param fileName JSON file containing the messages
         * @return pointer to a SyntaxMessages object
         */
        static std::shared_ptr<SyntaxMessages> loadMessagesFromFile(std::string fileName);

        /**
         * Load messages from string
         * @param data JSON data
         * @return pointer to a SyntaxMessages object
         */
        static std::shared_ptr<SyntaxMessages> loadMessagesFromString(std::string data);

        /**
         * Get error message
         * @param error token name
         * @return specific error message or default one
         */
        std::string getErrorMessage(std::string nonTerminal, std::string terminal);
    private:
        std::string m_defaultMessage;
        std::map<std::string, std::map<std::string, std::string>> m_errorMessages;

        // Reserved token names
        const std::string DEFAULT_TERMINAL_AND_NON_TERMINAL = ":any";

        /**
         * Load messages
         * @param document
         * @return pointer to a SyntaxMessages object
         */
        static std::shared_ptr<SyntaxMessages> loadMessages(rapidjson::Document &d);
    };
}

#endif

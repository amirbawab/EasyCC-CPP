#ifndef EASYCC_CPP_SYNTAXMESSAGES_H
#define EASYCC_CPP_SYNTAXMESSAGES_H

#include <map>
#include <string>
#include <memory>

namespace ecc{
    class SyntaxMessages {
    public:

        /**
         * Load messages from file
         * @param fileName JSON file containing the messages
         * @return pointer to a SyntaxMessages object
         */
        static std::shared_ptr<SyntaxMessages> loadMessages(std::string fileName);

        /**
         * Get error message
         * @param error token name
         * @return specific error message or default one
         */
        std::string getErrorMessage(std::string nonTerminal, std::string terminal);
    private:
        std::string defaultMessage;
        std::map<std::string, std::map<std::string, std::string>> errorMessages;

        // Reserved token names
        const std::string DEFAULT_TERMINAL_AND_NON_TERMINAL = ":any";
    };
}

#endif

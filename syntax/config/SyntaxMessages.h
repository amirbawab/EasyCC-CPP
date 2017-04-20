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
    private:
        std::string defaultMessage;
        std::map<std::string, std::map<std::string, std::string>> errorMessages;
    };
}

#endif

#ifndef EASYCC_CPP_LEXICALMESSAGES_H
#define EASYCC_CPP_LEXICALMESSAGES_H

#include <map>
#include <string>
#include <memory>
#include <rapidjson/document.h>

namespace ecc{
    class LexicalMessages {
    public:

        /**
         * Load messages from file
         * @param fileName JSON file containing the messages
         * @return pointer to a LexicalMessages object
         */
        static std::shared_ptr<LexicalMessages> loadMessagesFromFile(std::string fileName);

        /**
         * Load messages from string
         * @param fileName JSON string containing the messages
         * @return pointer to a LexicalMessages object
         */
        static std::shared_ptr<LexicalMessages> loadMessagesFromString(std::string data);

        /**
         * Get error message
         * @param error token name
         * @return specific error message or default one
         */
        std::string getErrorMessage(std::string errorTokenName);
    private:
        std::map<std::string,std::string> m_errorMessages;
        std::string m_defaultMessage;

        /**
         * Load messages
         * @param document
         * @return pointer to a LexicalMessages object
         */
        static std::shared_ptr<LexicalMessages> loadMessages(rapidjson::Document &document);
    };
}


#endif

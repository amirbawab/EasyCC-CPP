#ifndef EASYCC_CPP_LEXICALMESSAGES_H
#define EASYCC_CPP_LEXICALMESSAGES_H

#include <map>
#include <string>
#include <memory>

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
         * Get error message
         * @param error token name
         * @return specific error message or default one
         */
        std::string getErrorMessage(std::string errorTokenName);
    private:
        std::map<std::string,std::string> m_errorMessages;
        std::string m_defaultMessage;
    };
}


#endif

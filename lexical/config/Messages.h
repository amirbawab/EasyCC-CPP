#ifndef EASYCC_CPP_MESSAGES_H
#define EASYCC_CPP_MESSAGES_H

#include <map>
#include <string>
#include <memory>

namespace ecc{
    class Messages {
    public:

        /**
         * Load messages from file
         * @param fileName JSON file containing the messages
         * @return pointer to a Messages object
         */
        static std::shared_ptr<Messages> loadMessages(std::string fileName);

        /**
         * Get error message
         * @param error token name
         * @return specific error message or default one
         */
        std::string getMessage(std::string errorTokenName);
    private:
        std::map<std::string,std::string> errorMessages;
        std::string defaultMessage;
    };
}


#endif

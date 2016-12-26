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
        std::shared_ptr<Messages> loadMessages(std::string fileName);

        std::string getMessage(std::string key);
    private:
        std::map<std::string,std::string> messages;
        std::string defaultMessage;
    };
}


#endif

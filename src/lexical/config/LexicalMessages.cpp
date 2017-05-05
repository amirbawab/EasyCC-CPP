#include <easycc/LexicalMessages.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <sstream>

namespace ecc{
    std::shared_ptr<LexicalMessages> LexicalMessages::loadMessagesFromFile(std::string fileName) {

        // Configuration JSON format
        const char* ERROR_MESSAGES = "error_messages";
        const char* DEFAULT_MESSAGE = "default_message";

        // Load file into string stream
        std::ifstream file(fileName);
        std::stringstream  buffer;
        buffer << file.rdbuf();

        // Prepare a new LexicalMessage
        std::shared_ptr<LexicalMessages> messages = std::make_shared<LexicalMessages>();

        // Parse json
        rapidjson::Document d;
        d.Parse(buffer.str().c_str());

        // Default message
        messages->m_defaultMessage = d[DEFAULT_MESSAGE].GetString();

        // Loop on all messages
        for(auto i=d[ERROR_MESSAGES].MemberBegin(); i!=d[ERROR_MESSAGES].MemberEnd(); i++) {
            if(messages->m_errorMessages.count(i->name.GetString()) == 1) {
                throw std::runtime_error(std::string("Message of key ") + i->value.GetString() +
                                                 " is defined multiple times in " + fileName);
            }
            messages->m_errorMessages[i->name.GetString()] = i->value.GetString();
        }
        return messages;
    }

    std::string LexicalMessages::getErrorMessage(std::string key) {
        if(this->m_errorMessages.count(key) == 1) {
            return this->m_errorMessages[key];
        }
        return this->m_defaultMessage;
    }
}

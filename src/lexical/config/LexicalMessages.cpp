#include <easycc/LexicalMessages.h>
#include <rapidjson/writer.h>
#include <rapidjson/istreamwrapper.h>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <sstream>

namespace ecc{

    std::shared_ptr<LexicalMessages> LexicalMessages::loadMessages(rapidjson::Document &d) {

        // Configuration JSON format
        const char* ERROR_MESSAGES = "error_messages";
        const char* DEFAULT_MESSAGE = "default_message";

        // Prepare a new LexicalMessage
        std::shared_ptr<LexicalMessages> messages = std::make_shared<LexicalMessages>();

        // Default message
        messages->m_defaultMessage = d[DEFAULT_MESSAGE].GetString();

        // Loop on all messages
        for(auto i=d[ERROR_MESSAGES].MemberBegin(); i!=d[ERROR_MESSAGES].MemberEnd(); i++) {
            if(messages->m_errorMessages.count(i->name.GetString()) == 1) {
                throw std::runtime_error(std::string("Message of key ") + i->value.GetString() +
                                         " is defined multiple times");
            }
            messages->m_errorMessages[i->name.GetString()] = i->value.GetString();
        }
        return messages;
    }

    std::shared_ptr<LexicalMessages> LexicalMessages::loadMessagesFromFile(std::string fileName) {
        std::ifstream ifs(fileName);
        rapidjson::IStreamWrapper isw(ifs);
        rapidjson::Document d;
        d.ParseStream(isw);
        return loadMessages(d);
    }

    std::shared_ptr<LexicalMessages> LexicalMessages::loadMessagesFromString(std::string data) {
        rapidjson::Document d;
        d.Parse(data.c_str());
        return loadMessages(d);
    }

    std::string LexicalMessages::getErrorMessage(std::string key) {
        if(this->m_errorMessages.count(key) == 1) {
            return this->m_errorMessages[key];
        }
        return this->m_defaultMessage;
    }
}

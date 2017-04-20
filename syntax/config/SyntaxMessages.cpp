#include "SyntaxMessages.h"
#include "../../rapidjson/document.h"
#include "../../rapidjson/writer.h"
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <sstream>

#include <iostream>

namespace ecc {

    std::shared_ptr<SyntaxMessages> SyntaxMessages::loadMessages(std::string fileName) {

        // Configuration JSON format
        const char* ERROR_MESSAGES = "error_messages";
        const char* DEFAULT_MESSAGE = "default_message";
        const char* NON_TERMINAL = "non_terminal";
        const char* TERMINAL = "terminal";
        const char* MESSAGE = "message";

        // Load file into string stream
        std::ifstream file(fileName);
        std::stringstream  buffer;
        buffer << file.rdbuf();

        // Prepare a new Config
        std::shared_ptr<SyntaxMessages> messages = std::make_shared<SyntaxMessages>();

        // Parse json
        rapidjson::Document d;
        d.Parse(buffer.str().c_str());

        // Default message
        messages->defaultMessage = d[DEFAULT_MESSAGE].GetString();

        // Loop on all messages
        for(rapidjson::SizeType i=0; i < d[ERROR_MESSAGES].Size(); i++) {

            // Get error block information
            std::string nonTerminal = d[ERROR_MESSAGES][i][NON_TERMINAL].GetString();
            std::string terminal = d[ERROR_MESSAGES][i][TERMINAL].GetString();
            std::string message = d[ERROR_MESSAGES][i][MESSAGE].GetString();

            // Verify duplicates
            if(messages->errorMessages.find(nonTerminal) != messages->errorMessages.end() &&
                    messages->errorMessages[nonTerminal].find(terminal) != messages->errorMessages[nonTerminal].end()) {
                throw std::runtime_error(std::string("Message with non-terminal: ") + nonTerminal +
                                         " and terminal: " + terminal + " is defined multiple times in " + fileName);
            }
            messages->errorMessages[nonTerminal][terminal] = message;
        }
        return messages;
    }

    std::string SyntaxMessages::getErrorMessage(std::string nonTerminal, std::string terminal) {
        if(errorMessages.find(nonTerminal) != errorMessages.end()) {
                if(errorMessages[nonTerminal].find(terminal) != errorMessages[nonTerminal].end()) {
                    return errorMessages[nonTerminal][terminal];
                } else if(errorMessages[nonTerminal].find(NON_TERMINAL_DEFAULT_TERMINAL) !=
                        errorMessages[nonTerminal].end()) {
                    return errorMessages[nonTerminal][NON_TERMINAL_DEFAULT_TERMINAL];
                }
        }
        return this->defaultMessage;
    }
}

#ifndef EASYCC_CPP_SYNTAXMESSAGE_H
#define EASYCC_CPP_SYNTAXMESSAGE_H

namespace ecc{
    class SyntaxMessage {
    public:

        /**
         * Load messages from file
         * @param fileName JSON file containing the messages
         * @return pointer to a Messages object
         */
        static std::shared_ptr<SyntaxMessages> loadMessages(std::string fileName);

    };
}

#endif

#ifndef EASYCC_CPP_LEXICALTOKEN_H
#define EASYCC_CPP_LEXICALTOKEN_H

#include <string>
#include <ostream>

namespace ecc {
    class LexicalToken {
    public:
        static const std::string END_OF_FILE;

        // Set the token types
        enum Type {
            ERROR_TOKEN,
            NORMAL_TOKEN,
            NORMAL_FINAL_TOKEN
        };

        LexicalToken(LexicalToken::Type type,std::string name, std::string value, int line, int column, int position) :
                type(type), name(name), value(value), line(line), column(column), position(position){};

        /**
         * Get token name
         * @return token name
         */
        std::string getName() const {return this->name;}

        /**
         * Get token value
         * @return token value
         */
        std::string getValue() const {return this->value;}

        /**
         * Get token line
         * @return line
         */
        int getLine() const {return this->line;}

        /**
         * Get token column
         * @return column
         */
        int getColumn() const {return this->column;}

        /**
         * Get token position
         * @return position
         */
        int getPosition() const {return this->position;}

        /**
         * Get lexical token type
         * @return lexical token type
         */
        LexicalToken::Type getType() const {return this->type;}

    private:
        std::string name;
        std::string value;
        int line;
        int column;
        int position;
        LexicalToken::Type type;

        /**
         * To string
         */
        friend std::ostream& operator<<(std::ostream& os, const LexicalToken &lexicalToken) {
            os << "<" << lexicalToken.name << ",l:" << lexicalToken.line << ",c:" << lexicalToken.column << ",p:" << lexicalToken.position << ">";
            return os;
        }
    };
}


#endif

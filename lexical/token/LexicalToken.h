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
                m_type(type), m_name(name), m_value(value), m_line(line), m_column(column), m_position(position){};

        /**
         * Get token name
         * @return token name
         */
        std::string getName() const {return this->m_name;}

        /**
         * Get token value
         * @return token value
         */
        std::string getValue() const {return this->m_value;}

        /**
         * Get token line
         * @return line
         */
        int getLine() const {return this->m_line;}

        /**
         * Get token column
         * @return column
         */
        int getColumn() const {return this->m_column;}

        /**
         * Get token position
         * @return position
         */
        int getPosition() const {return this->m_position;}

        /**
         * Get lexical token type
         * @return lexical token type
         */
        LexicalToken::Type getType() const {return this->m_type;}

    private:
        std::string m_name;
        std::string m_value;
        int m_line;
        int m_column;
        int m_position;
        LexicalToken::Type m_type;

        /**
         * To string
         */
        friend std::ostream& operator<<(std::ostream& os, const LexicalToken &lexicalToken) {
            os << "<" << lexicalToken.m_name << ",l:" << lexicalToken.m_line << ",c:"
            << lexicalToken.m_column << ",p:" << lexicalToken.m_position << ">";
            return os;
        }
    };
}


#endif

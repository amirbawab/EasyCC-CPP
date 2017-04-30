#ifndef LEXICAL_GRAPH_STATE_H_
#define LEXICAL_GRAPH_STATE_H_

#include <string>
#include <map>
#include <memory>
#include <ostream>

namespace ecc {
    class State {
    public:

        // State type
        static const std::string INITIAL;
        static const std::string NORMAL;
        static const std::string FINAL;

        State(int id, std::string type) :
                m_id(id), m_type(type), m_backtrack(false){}
        State(int id, std::string type, std::string tokenName, bool backtrack) :
                m_type(type), m_id(id), m_tokenName(tokenName), m_backtrack(backtrack){}

        /**
         * Get state id
         * @return state id
         */
        int getId() const {return m_id;}

        /**
         * Get token name
         * @return token name
         */
        std::string getTokenName() const {return m_tokenName;}

        /**
         * Check backtrack value for state
         * @return true|false
         */
        bool mustBacktrack() const {return this->m_backtrack;}

        /**
         * Get state type
         * @return type as enum of STATE
         */
        const std::string getType() const {return this->m_type;}

    private:

        // Set the type of the state
        std::string m_type;

        // Unique id for each state
        int m_id;

        // Token name (for final states)
        std::string m_tokenName;

        // Check if parser must backtrack (for final states)
        bool m_backtrack;

        /**
         * To string
         */
        friend std::ostream& operator<<(std::ostream& os, const State &state) {
            os << state.getId();
            os << " (" << state.getType() << ")" ;
            return os;
        }
    };
}

#endif

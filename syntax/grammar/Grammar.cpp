#include "Grammar.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/trim.hpp>

namespace ecc {
    Grammar::Grammar(std::string grammarFile) {

        // Load file
        std::ifstream file(grammarFile);

        // Read line by line
        std::string line;
        while (std::getline(file, line)) {
            std::vector<std::string> words;
            boost::split(words, line, boost::is_any_of(":"), boost::token_compress_on);

            // If two, then it's a complete definition
            if(words.size() == 2) {

                // Clear whitespaces on both sides
                boost::trim(words[0]);

                // Check if non-terminal is composed of upper case alphabets only
                for(size_t i=0; i < words[0].size(); i++) {
                    if((words[0][i] < 'A' || words[0][i] > 'Z') && words[0][i] != '_') {
                        throw std::runtime_error("Non terminals should be composed of upper case letters only.");
                    }
                }
            }
        }
    }
}

#include "Grammar.h"
#include <fstream>
#include <sstream>

namespace ecc {
    Grammar::Grammar(std::string grammarFile) {

        // Load file into string stream
        std::ifstream file(grammarFile);
        std::stringstream  buffer;
        buffer << file.rdbuf();

    }
}

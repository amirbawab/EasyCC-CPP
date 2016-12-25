#ifndef LEXICAL_LEXICAL_H_
#define LEXICAL_LEXICAL_H_

#include <memory>
#include <string>
#include "graph/Graph.h"
#include "config/Config.h"

namespace ecc {

    class Lexical {
    public:
        Lexical(std::string stateMachineFileName, std::string configFileName);
    private:
        std::shared_ptr<Graph> graph;
        std::shared_ptr<Config> config;
    };
}

#endif

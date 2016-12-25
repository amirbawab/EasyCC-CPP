#include "Lexical.h"

namespace ecc {
    Lexical::Lexical(std::string stateMachineFileName, std::string configFileName) {
        this->graph = Graph::buildGraph(stateMachineFileName);
        this->config = Config::buildConfig(configFileName);
    }
}

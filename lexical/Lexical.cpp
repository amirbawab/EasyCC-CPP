#include "Lexical.h"

namespace ecc {
    Lexical::Lexical(std::string stateMachineFileName) {
        this->graph = Graph::buildGraph(stateMachineFileName);
    }
}

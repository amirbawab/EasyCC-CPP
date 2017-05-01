#include "easycc/EasyCC.h"
#include <iostream>

#include <stack>
#include <fstream>

typedef std::vector<std::shared_ptr<ecc::LexicalToken>> Tokens;

int main(int argc, char *argv[]) {
    ecc::EasyCC easyCC;

    int code;

    // Initialize class
    code = easyCC.init(argc, argv);
    if(code != ecc::EasyCC::OK_CODE) {
        return code;
    }

    // Create file
    std::ofstream outputFile(easyCC.getOutputFileName());

    // Check if can open file
    if(!outputFile.is_open()) {
        std::cerr << "Cannot open file!" << std::endl;
        return 1;
    }

    // Create stack for computing the expression
    std::stack<int> calc;

    // Register events
    easyCC.registerSemanticAction("#push#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        calc.push(std::stoi(lexicalVector[index]->getValue()));
    });

    easyCC.registerSemanticAction("#plus#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        int popR = calc.top();
        calc.pop();
        int popL = calc.top();
        calc.pop();
        calc.push(popL + popR);
    });

    easyCC.registerSemanticAction("#minus#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        int popR = calc.top();
        calc.pop();
        int popL = calc.top();
        calc.pop();
        calc.push(popL - popR);
    });

    easyCC.registerSemanticAction("#multiply#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        int popR = calc.top();
        calc.pop();
        int popL = calc.top();
        calc.pop();
        calc.push(popL * popR);
    });

    easyCC.registerSemanticAction("#divide#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        int popR = calc.top();
        calc.pop();
        int popL = calc.top();
        calc.pop();
        calc.push(popL / popR);
    });

    easyCC.registerSemanticAction("#print#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        outputFile << "Expression result: " << calc.top() << std::endl;
        calc.pop();
    });

    easyCC.registerSemanticAction("#end#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        outputFile.close();
    });

    // Start compiling
    code = easyCC.compile();
    return code;
}

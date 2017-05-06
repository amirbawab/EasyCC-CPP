#include <easycc/EasyCCPro.h>
#include <iostream>
#include <stack>

typedef std::vector<std::shared_ptr<ecc::LexicalToken>> Tokens;

int main(int argc, char *argv[]) {
    ecc::EasyCC easyCC;

    // Create stack for computing the expression
    std::stack<int> calc;

    // Check if expression evaluation is stable
    bool noError = true;

    /**
     * Register 'push' semantic action
     * Every time an integer is read it will be pushed into the stack
     */
    easyCC.registerSemanticAction("#push#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        noError &= stable;
        if(noError) {
            calc.push(std::stoi(lexicalVector[index]->getValue()));
        }
    });

    /**
     * Register 'plus' semantic action
     * Once the two operands are pushed, add them and push the result into the stack
     */
    easyCC.registerSemanticAction("#plus#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        noError &= stable;
        if(noError) {
            int popR = calc.top();
            calc.pop();
            int popL = calc.top();
            calc.pop();
            calc.push(popL + popR);
        }
    });

    /**
     * Register 'minus' semantic action
     * Once the two operands are pushed, subtract them and push the result into the stack
     */
    easyCC.registerSemanticAction("#minus#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        noError &= stable;
        if(noError) {
            int popR = calc.top();
            calc.pop();
            int popL = calc.top();
            calc.pop();
            calc.push(popL - popR);
        }
    });

    /**
     * Register 'multiply' semantic action
     * Once the two operands are pushed, multiply them and push the result into the stack
     */
    easyCC.registerSemanticAction("#multiply#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        noError &= stable;
        if(noError) {
            int popR = calc.top();
            calc.pop();
            int popL = calc.top();
            calc.pop();
            calc.push(popL * popR);
        }
    });

    /**
     * Register 'divide' semantic action
     * Once the two operands are pushed, divide them and push the result into the stack
     */
    easyCC.registerSemanticAction("#divide#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        noError &= stable;
        if(noError) {
            int popR = calc.top();
            calc.pop();
            int popL = calc.top();
            calc.pop();
            calc.push(popL / popR);
        }
    });

    /**
     * Register 'print' semantic action
     * At the end of the expression, output the result to a file
     */
    easyCC.registerSemanticAction("#print#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        if(noError) {
            std::cout << "Expression result: " << calc.top() << std::endl;
        } else {
            std::cout << "Expression has a mistake." << std::endl;
        }

        // Reset error flag
        noError = true;

        // Clear stack for another expression
        while(!calc.empty()) {
            calc.pop();
        }
    });

    /**
     * Register 'end' semantic action
     * Once the end of file is reached, close the output file
     */
    easyCC.registerSemanticAction("#end#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        std::cout.flush();
    });

    // No need for more than one phase for this example, so let's pick phase 0
    // The phase number selected is important because the semantic action handlers
    // will hold the phase number in which the action occurred
    easyCC.setParsingPhase(0);

    // Start compiling
    for(int i=1; i < argc; i++) {
        int code = easyCC.compile(argv[i]);
        if(code != ecc::EasyCC::OK_CODE) {
            return code;
        }
    }

    return 0;
}
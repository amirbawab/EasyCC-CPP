#include <calculator/Calculator.h>

void Calculator::setOutput(std::string filename) {
    m_output.open(filename);
}

void Calculator::error() {
    m_output << "Error evaluating expression" << std::endl;
    m_output.close();
}

void Calculator::initHandlers() {
    /**
     * Register 'push' semantic action
     * Every time an integer is read it will be pushed into the stack
     */
    m_easyCC->registerSemanticAction("#push#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        m_operands.push(std::stoi(lexicalVector[index]->getValue()));
    });

    /**
     * Register 'plus' semantic action
     * Once the two operands are pushed, add them and push the result into the stack
     */
    m_easyCC->registerSemanticAction("#plus#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        int popR = m_operands.top();
        m_operands.pop();
        int popL = m_operands.top();
        m_operands.pop();
        m_operands.push(popL + popR);
    });

    /**
     * Register 'minus' semantic action
     * Once the two operands are pushed, subtract them and push the result into the stack
     */
    m_easyCC->registerSemanticAction("#minus#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        int popR = m_operands.top();
        m_operands.pop();
        int popL = m_operands.top();
        m_operands.pop();
        m_operands.push(popL - popR);
    });

    /**
     * Register 'multiply' semantic action
     * Once the two operands are pushed, multiply them and push the result into the stack
     */
    m_easyCC->registerSemanticAction("#multiply#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        int popR = m_operands.top();
        m_operands.pop();
        int popL = m_operands.top();
        m_operands.pop();
        m_operands.push(popL * popR);
    });

    /**
     * Register 'divide' semantic action
     * Once the two operands are pushed, divide them and push the result into the stack
     */
    m_easyCC->registerSemanticAction("#divide#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        int popR = m_operands.top();
        m_operands.pop();
        int popL = m_operands.top();
        m_operands.pop();
        m_operands.push(popL / popR);
    });

    /**
     * Register 'print' semantic action
     * At the end of the expression, output the result to a file
     */
    m_easyCC->registerSemanticAction("#print#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        m_output << "Expression result: " << m_operands.top() << std::endl;
        m_operands.pop();
    });

    /**
     * Register 'end' semantic action
     * Once the end of file is reached, close the output file
     */
    m_easyCC->registerSemanticAction("#end#",[&](int phase, Tokens &lexicalVector, int index, bool stable){
        m_output.close();
    });
}


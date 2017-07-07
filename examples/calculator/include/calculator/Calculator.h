#ifndef BASHCLASS_CALCULATOR_H
#define BASHCLASS_CALCULATOR_H

#include <easycc/IEasyCC.h>
#include <fstream>
#include <stack>

typedef std::vector<std::shared_ptr<ecc::LexicalToken>> Tokens;

class Calculator {
public:
    /**
     * Initialize calculator semantic anction handlers
     */
    void initHandlers();

    /**
     * Set easycc
     * @param easycc
     */
    void setEasyCC(std::shared_ptr<ecc::IEasyCC> easycc) {m_easyCC = easycc;}

    /**
     * Set output file
     * @param filename
     */
    void setOutput(std::string filename);
private:

    // Create stack for computing the expression
    std::stack<int> m_operands;

    // Hold an instance of easycc
    std::shared_ptr<ecc::IEasyCC> m_easyCC;

    // Output file
    std::ofstream m_output;
};

#endif

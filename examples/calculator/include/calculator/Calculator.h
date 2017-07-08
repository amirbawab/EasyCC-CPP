#ifndef BASHCLASS_CALCULATOR_H
#define BASHCLASS_CALCULATOR_H

#include <easycc/IEasyCC.h>
#include <fstream>
#include <stack>

typedef std::vector<std::shared_ptr<ecc::LexicalToken>> Tokens;

class Calculator {
public:

    /**
     * Initialize members
     * @param easycc
     */
    Calculator(std::shared_ptr<ecc::IEasyCC> easycc) : m_easyCC(easycc){}

    /**
     * Initialize calculator semantic anction handlers
     */
    void initHandlers();

    /**
     * Compile files
     * @param inputFiles
     * @param outputFile
     */
    int compile(std::vector<std::string> inputFiles, std::string outputFile);
private:

    // Create stack for computing the expression
    std::stack<int> m_operands;

    // Hold an instance of easycc
    std::shared_ptr<ecc::IEasyCC> m_easyCC;

    // Output file
    std::ofstream m_output;
};

#endif

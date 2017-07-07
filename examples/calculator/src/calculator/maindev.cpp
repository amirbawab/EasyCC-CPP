#include <easycc/EasyCCDev.h>
#include <calculator/Calculator.h>
#include <iostream>
#include <getopt.h>

int main(int argc, char *argv[]) {

    // Create easycc production mode
    std::shared_ptr<ecc::EasyCC> easyccdev = std::make_shared<ecc::EasyCC>();

    int code;
    code = easyccdev->init(argc, argv);
    if(code != ecc::EasyCC::OK_CODE) {
        return code;
    }

    // Create calculator
    Calculator calculator;

    // Set calculator easycc
    calculator.setEasyCC(easyccdev);

    // Initialize semantic action handlers
    calculator.initHandlers();

    // Set output file
    calculator.setOutput(easyccdev->getOutputFileName());

    // Configure easycc
    easyccdev->setParsingPhase(0);
    easyccdev->setSilentSyntaxErrorMessages(false);
    easyccdev->setSilentSemanticEvents(false);
    easyccdev->setOnSyntaxError([&](){
        easyccdev->setSilentSemanticEvents(true);
    });

    // Compile all files
    for(std::string fileName : easyccdev->getInputFilesNames()) {
        code = easyccdev->compile(fileName);
        if(code != ecc::EasyCC::OK_CODE) {
            return code;
        }
    }

    return 0;
}

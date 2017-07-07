#include <easycc/EasyCCPro.h>
#include <calculator/Calculator.h>
#include <iostream>
#include <getopt.h>

// Hold output file
std::string outputFile;

// Hold input files
std::vector<std::string> inputFiles;

/**
 * Print program usage
 */
void printUsage() {
    std::cout
            << "calculator - EasyCC example" << std::endl
            << "Usage: calculator [OPTION]... [FILE]..." << std::endl
            << "\t-o, --output\t\t\tOutput file" << std::endl
            << "\t-h, --help\t\t\tDisplay this help message" << std::endl;
}

/**
 * Initialize parameter
 * @param argc
 * @param argv
 */
void initParams(int argc, char *argv[]) {

    struct option longOptions[] = {
            {"output", required_argument, 0, 'o'},
            {"help",   no_argument,       0, 'h'},
            {0,        0,                 0, 0}
    };

    int optionIndex = 0;
    int c;
    while ((c = getopt_long(argc, argv, "ho:", longOptions, &optionIndex)) != -1) {
        switch (c) {
            case 'o':
                outputFile = optarg;
                break;
            case 'h':
            default:
                // Print by default
                break;
        }
    }
}


int main(int argc, char *argv[]) {

    // Store input files in a vector
    for(int i = optind; i < argc; ++i) {
        inputFiles.push_back(argv[i]);
    }

    // Initialize parameters
    initParams(argc, argv);

    // Check missing requirements
    if(outputFile.empty() || inputFiles.empty()) {
        printUsage();
        return ecc::EasyCC::ERR_CODE_PARAMS;
    }

    // Create easycc production mode
    std::shared_ptr<ecc::IEasyCC> easyccpro = std::make_shared<ecc::EasyCC>();

    // Create calculator
    Calculator calculator;

    // Set calculator easycc
    calculator.setEasyCC(easyccpro);

    // Initialize semantic action handlers
    calculator.initHandlers();

    // Set output file
    calculator.setOutput(outputFile);

    // Configure easycc
    easyccpro->setParsingPhase(0);
    easyccpro->setSilentSyntaxErrorMessages(false);
    easyccpro->setSilentSemanticEvents(false);
    easyccpro->setOnSyntaxError([&](){
        easyccpro->setSilentSemanticEvents(true);
        calculator.error();
    });

    // Compile all files
    for(std::string fileName : inputFiles) {
        int code = easyccpro->compile(fileName);
        if(code != ecc::EasyCC::OK_CODE) {
            return code;
        }
    }

    return 0;
}

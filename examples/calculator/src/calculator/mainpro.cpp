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

    // Initialize parameters
    initParams(argc, argv);

    // Store input files in a vector
    for(int i = optind; i < argc; ++i) {
        inputFiles.push_back(argv[i]);
    }

    // Check missing requirements
    if(outputFile.empty() || inputFiles.empty()) {
        printUsage();
        return ecc::EasyCC::ERR_CODE_PARAMS;
    }

    // Create easycc production mode
    std::shared_ptr<ecc::IEasyCC> easyccpro = std::make_shared<ecc::EasyCC>();

    // Create calculator
    Calculator calculator(easyccpro);

    // Compile files
    return calculator.compile(inputFiles, outputFile);
}

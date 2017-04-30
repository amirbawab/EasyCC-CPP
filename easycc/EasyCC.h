#ifndef EASYCC_EASYCC_H
#define EASYCC_EASYCC_H

#include <string>
#include <vector>

namespace ecc{
    class EasyCC {
    public:

        // Return codes
        static const int OK_CODE = 0;
        static const int ERR_CODE_PARAMS = 1;
        static const int ERR_CODE_LEXICAL = 2;
        static const int ERR_CODE_SYNTAX = 3;

        /**
         * Run the lexical analyzer and the syntax analyzer
         * @param argc
         * @param argv
         * @return Return code
         */
        int compile(int argc, char *argv[]);
    private:

        // Configuration files
        std::string lexicalStateMachineFile;
        std::string lexicalConfigFile;
        std::string lexicalErrorsFile;
        std::string syntaxGrammarFile;
        std::string syntaxConfigFile;
        std::string syntaxErrorsFile;
        std::string outputFile;
        std::vector<std::string> inputFiles;

        /**
         * Initiliaze the logger
         */
        void initLogs();

        /**
         * Print the program usage
         */
        void printUsage();

        /**
         * Initialize/populate program paramters
         * @param argc
         * @param argv
         */
        void initParams(int argc, char *argv[]);

        /**
         * Check if the required argument are initialized
         */
        bool validArguments();

        /**
         * Store the path for the input files
         * @param argc
         * @param argv
         */
        void fetchInputFiles(const int argc, char *argv[]);
    };
}

#endif

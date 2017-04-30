#ifndef EASYCC_EASYCC_H
#define EASYCC_EASYCC_H

#include <string>
#include <vector>
#include <memory>
#include "../lexical/Lexical.h"
#include "../syntax/Syntax.h"

namespace ecc{
    class EasyCC {
    public:

        // Return codes
        static const int OK_CODE = 0;
        static const int ERR_CODE_PARAMS = 1;
        static const int ERR_CODE_LEXICAL = 2;
        static const int ERR_CODE_SYNTAX = 3;

        /**
         * Init logs and other tools
         * @param argc
         * @param argv
         */
        int init(int argc, char *argv[]);

        /**
         * Run the lexical analyzer and the syntax analyzer
         * @return Return code
         */
        int compile();

        /**
         * Register semantic action
         * @param semanticAction
         * @param semanticActionFunction Function to execute on semantic action calls
         */
        void registerSemanticAction(std::string semanticAction, std::function<void
                (int, std::vector<std::shared_ptr<LexicalToken>>&, int, bool)> semanticActionFunction);
    private:

        // Compiler analyzers
        std::shared_ptr<Lexical> lexical;
        std::shared_ptr<Syntax> syntax;

        // Configuration files
        std::string lexicalStateMachineFile;
        std::string lexicalConfigFile;
        std::string lexicalErrorsFile;
        std::string syntaxGrammarFile;
        std::string syntaxConfigFile;
        std::string syntaxErrorsFile;
        std::string outputFile;
        std::vector<std::string> inputFiles;
        std::map<std::string, std::function<void
                (int, std::vector<std::shared_ptr<LexicalToken>>&, int, bool)>> semanticActionMap;

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

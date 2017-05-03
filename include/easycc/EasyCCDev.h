#ifndef EASYCC_EASYCCDEV_H
#define EASYCC_EASYCCDEV_H

#include <easycc/IEasyCC.h>
#include <string>
#include <vector>
#include <memory>
#include <easycc/Lexical.h>
#include <easycc/Syntax.h>

namespace ecc{
    class EasyCC : public IEasyCC {
    public:

        /**
         * Init logs and other tools
         * @param argc
         * @param argv
         */
        int init(int argc, char *argv[]);

        /**
         * Get output file name
         * @return file name
         */
        std::string getOutputFileName() {return m_outputFile; }

        /**
         * Get input files names
         * @return input files names
         */
        std::vector<std::string> getInputFilesNames() {return m_inputFiles;}
    private:

        // Configuration files
        std::string m_lexicalStateMachineFile;
        std::string m_lexicalConfigFile;
        std::string m_lexicalErrorsFile;
        std::string m_syntaxGrammarFile;
        std::string m_syntaxConfigFile;
        std::string m_syntaxErrorsFile;
        std::string m_outputFile;
        std::vector<std::string> m_inputFiles;

        /**
         * Initialize the logger
         */
        void initLogs();

        /**
         * Print the program usage
         */
        void printUsage();

        /**
         * Initialize/populate program parameters
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

#include "easycc/EasyCC.h"
#include <iostream>

int main(int argc, char *argv[]) {
    ecc::EasyCC easyCC;

    int code;

    // Initialize class
    code = easyCC.init(argc, argv);
    if(code != ecc::EasyCC::OK_CODE) {
        return code;
    }

    // Get file name
    std::string outputFileName = easyCC.getOutputFileName();

    // Register events
    easyCC.registerSemanticAction("#className#",[&](int phase,
                                         std::vector<std::shared_ptr<ecc::LexicalToken>> &lexicalVector,
                                         int index, bool stable){
        std::cout << "Class name " << lexicalVector[index]->getValue() << std::endl;
    });

    // Start compiling
    code = easyCC.compile();
    return code;
}

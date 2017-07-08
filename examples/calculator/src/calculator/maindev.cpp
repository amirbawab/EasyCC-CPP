#include <easycc/EasyCCDev.h>
#include <calculator/Calculator.h>

int main(int argc, char *argv[]) {

    // Create easycc production mode
    std::shared_ptr<ecc::EasyCC> easyccdev = std::make_shared<ecc::EasyCC>();

    int code;
    code = easyccdev->init(argc, argv);
    if(code != ecc::EasyCC::OK_CODE) {
        return code;
    }

    // Create calculator
    Calculator calculator(easyccdev);

    // Compile files
    return calculator.compile(easyccdev->getInputFilesNames(), easyccdev->getOutputFileName());
}

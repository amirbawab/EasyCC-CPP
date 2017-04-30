#include "easycc/EasyCC.h"

int main(int argc, char *argv[]) {
    ecc::EasyCC easyCC;
    int code = easyCC.compile(argc, argv);
    return code;
}

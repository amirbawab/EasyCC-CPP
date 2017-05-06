#include <easycc/EasyCCPro.h>
#include <iostream>

int main(int argc, char *argv[]) {
    ecc::EasyCC easyCC;

    for(int i=1; i < argc; i++) {
        easyCC.compile(argv[i]);
    }
    return 0;
}
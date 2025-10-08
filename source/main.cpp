#include "include/cli.h"
#include <iostream>

auto main(int  /*argc*/, char*  /*argv*/[]) -> int {
    try {
         CAS_CLI cli;
         cli.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
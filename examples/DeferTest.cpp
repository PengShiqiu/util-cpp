#include <iostream>
#include "util/defer/Defer.h"
#include "util/future/Future.h"

int main(void){

    ANANAS_DEFER{
        std::cout << "Exit RAII" << std::endl;
    };
    std::cout << "hello" << std::endl;

    return 0;
}
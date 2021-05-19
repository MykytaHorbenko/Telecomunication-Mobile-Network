#include "Wrapper_header.hpp"
#include <iostream>
int main(int argc, const char** argv) {

    Wrapper test;

    auto temp = test.opendir("/home/nikita/Test_Project");

    test.sleep(1000);

    std::cout<<"Hello World"<<std::endl;

    return 0;
}


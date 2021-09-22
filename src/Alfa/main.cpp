#include <iostream>
#include <string>
#include "config.hpp"

int main(int argc, char* argv[])
{
    std::cout << "Hello PH" << configuration::config << '\n';
    return 0;
}

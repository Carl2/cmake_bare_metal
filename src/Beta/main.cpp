#include <iostream>
#include <string>
#include "config.hpp"
#include "hal.hpp"

int main(int argc, char* argv[])
{
    std::cout << "Hello PH Beta using config: " << configuration::config << " " << '\n';
    HAL::blink();
    return 0;
}

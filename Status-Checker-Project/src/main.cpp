#include "status.hpp"
#include <iostream>

int main()
{
    if (!status_check())
        return 1;

    std::cout << "Hello, world!" << std::endl;
    return 0;
}
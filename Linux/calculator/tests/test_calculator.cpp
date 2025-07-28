#include "calculator.h"
#include <iostream>

int main()
{
    int a = 10, b = 5;

    if (calculator::add(a, b) == 15 && calculator::subtract(a, b) == 5)
    {
        std::cout << "Tests passed!" << std::endl;
    }
    else
    {
        std::cout << "Tests failed!" << std::endl;
    }

    return 0;
}
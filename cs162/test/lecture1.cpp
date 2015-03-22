#include <iostream>
#include <cmath>
#include "functions.hpp"

int main(int argc, char **argv)
{
    std::cout << std::sqrt(121) << std::endl;
    int a = 1, b = 2;
    std::string c = "a", d = "b";

    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;

    swap(a,b);
    swap(c,d);
    std::cout << "a = " << a << std::endl;
    std::cout << "b = " << b << std::endl;

    return 0;
}

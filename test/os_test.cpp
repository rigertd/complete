#include <iostream>

int main()
{
    #if defined(__linux__)
    std::cout << "Running on Linux." << std::endl;
    #endif
    #if defined(_WIN32)
    std::cout << "Running on Windows 32." << std::endl;
    #endif
    #if defined(_WIN64)
    std::cout << "Running on Windows 64-bit." << std::endl;
    #endif
    std::cin.get();
    return 0;
}
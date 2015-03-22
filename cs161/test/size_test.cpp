#include <string>
#include <iostream>

using namespace std;

int main()
{
    std::string test_str = "test";
    char test_arr[5] = "test";
    char test = 123;
    
    std::cout << "Size of string object: " << sizeof(test_str) << " bytes.\n";
    std::cout << "Size of c-style string: " << sizeof(test_arr[]) << " bytes.\n";
    for (int i = 0; i <10; i++)
    {
        std::cout << "C subscript " << i << ": " << static_cast<int>(test_arr[i]) << std::endl;
        std::cout << "C++ subscript " << i << ": " << static_cast<int>(test_str[i]) << std::endl;
    }
    
    std::cin.get();
    
    return 0;
}
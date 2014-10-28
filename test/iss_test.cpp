#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

int main()
{
    std::string testString = "This  \t is a    test";
    int strCount = 0;
    std::string str1, str2, str3, buffer;
    
    std::istringstream iss (testString);
    do
    {
        iss >> buffer;
        strCount++;
	} while (!iss.fail());
    strCount--;
    std::cout << testString << ": " << strCount;
    std::cin.get();
	return 0;
}


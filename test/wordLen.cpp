#include <iostream>

using namespace std;

int wordLength(char *, int = 0);

int main()
{
    char str[] = "12345";
    cout << wordLength(str) << endl;
    cin.get();
}

int wordLength(char *str, int pos)
{
    if (str[pos] == '\0')
        return 0;
    else
        return wordLength(str, pos + 1) + 1;
}
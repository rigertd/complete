#include "EncryptionFilter.hpp"

EncryptionFilter::EncryptionFilter(int key)
{
    this->key = key;
}

char EncryptionFilter::transform(char c)
{
    if (c >= 'A' && c <= 'Z')
        return (c - 'A' + key) % 26 + 'A';
    else if (c >= 'a' && c <= 'z')
        return (c - 'a' + key) % 26 + 'a';
    else
        return c;
}

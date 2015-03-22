/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/4/2015
 * Last Modified:   2/6/2015
 * Assignment:      Lab 5
 * Filename:        EncryptionFilter.cpp
 *
 * Description:     Implementation of the EncryptionFilter class.
 *                  Transforms the input file using a Caesar cipher.
 ************************************************************************/
#include "EncryptionFilter.hpp"

// Constructor requires the encryption key
EncryptionFilter::EncryptionFilter(int key)
{
    this->key = key;
}

// Transforms the specified char c.
// Increments the char value by the specified key value.
// Wraps around at end of alphabet. Does not change the case
// or non-letter characters.
char EncryptionFilter::transform(char c)
{
    if (c >= 'A' && c <= 'Z')
        return (c - 'A' + key) % 26 + 'A';
    else if (c >= 'a' && c <= 'z')
        return (c - 'a' + key) % 26 + 'a';
    else
        return c;
}

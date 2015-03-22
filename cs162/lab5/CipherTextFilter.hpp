/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/4/2015
 * Last Modified:   2/6/2015
 * Assignment:      Lab 5
 * Filename:        CipherTextFilter.hpp
 *
 * Description:     Represents a file filter that transforms the text of
 *                  one file into ciphertext in another file.
 *                  This class is derived from the EncryptionFilter class.
 ************************************************************************/
#ifndef CIPHER_TEXT_FILTER_HPP
#define CIPHER_TEXT_FILTER_HPP

#include "EncryptionFilter.hpp"

class CipherTextFilter : public EncryptionFilter
{
private:
    virtual char transform(char);
public:
    // pass encryption key to parent class unchanged
    CipherTextFilter(int key) : EncryptionFilter(key) { }
    // need to override doFilter to add a space every 5 chars
    virtual void doFilter(std::ifstream &, std::ofstream &);
};

#endif

/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/4/2015
 * Last Modified:   2/6/2015
 * Assignment:      Lab 5
 * Filename:        EncryptionFilter.hpp
 *
 * Description:     Represents a file filter that transforms the text of
 *                  a file into an encrypted file using a Caesar cipher.
 ************************************************************************/
#ifndef ENCRYPTION_FILTER_HPP
#define ENCRYPTION_FILTER_HPP

#include "Filter.hpp"

class EncryptionFilter : public Filter
{
protected:
    // use protected so that CipherTextFilter subclass can access these
    int key;    // value to shift letters by
    virtual char transform(char);
public:
    EncryptionFilter(int);
};

#endif

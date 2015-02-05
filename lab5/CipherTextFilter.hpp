#ifndef CIPHER_TEXT_FILTER_HPP
#define CIPHER_TEXT_FILTER_HPP

#include "EncryptionFilter.hpp"

class CipherTextFilter : public EncryptionFilter
{
private:
    virtual char transform(char);
public:
    CipherTextFilter(int key) : EncryptionFilter(key) { }
    virtual void doFilter(std::ifstream &, std::ofstream &);
};

#endif

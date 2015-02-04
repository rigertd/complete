#ifndef CIPHER_TEXT_FILTER_HPP
#define CIPHER_TEXT_FILTER_HPP

#include <fstream>
#include "Filter.hpp"

class CipherTextFilter : public Filter
{
private:
    virtual char transform(char);
};

#endif

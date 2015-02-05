#ifndef ENCRYPTION_FILTER_HPP
#define ENCRYPTION_FILTER_HPP

#include "Filter.hpp"

class EncryptionFilter : public Filter
{
protected:
    int key;
    virtual char transform(char);
public:
    EncryptionFilter(int);
};

#endif

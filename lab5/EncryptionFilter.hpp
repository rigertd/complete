#ifndef ENCRYPTION_FILTER_HPP
#define ENCRYPTION_FILTER_HPP

#include <fstream>
#include "Filter.hpp"

class EncryptionFilter : public Filter
{
private:
    int key;
    virtual char transform(char);
public:
    EncryptionFilter(int);
};

#endif

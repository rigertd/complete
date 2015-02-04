#ifndef UPPERCASE_FILTER_HPP
#define UPPERCASE_FILTER_HPP

#include <fstream>
#include "Filter.hpp"

class UppercaseFilter : public Filter
{
private:
    virtual char transform(char);
public:
    UppercaseFilter() {}
};

#endif

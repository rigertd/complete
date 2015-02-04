#ifndef COPY_FILTER_HPP
#define COPY_FILTER_HPP

#include <fstream>
#include "Filter.hpp"

class CopyFilter : public Filter
{
private:
    virtual char transform(char);
public:
    CopyFilter() {}
};

#endif

#ifndef FILTER_HPP
#define FILTER_HPP

#include <fstream>

class Filter
{
private:
    virtual char transform(char) = 0;
public:
    void doFilter(std::ifstream &, std::ofstream &);
};

#endif

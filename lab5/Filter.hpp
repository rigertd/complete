/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/4/2015
 * Last Modified:   2/6/2015
 * Assignment:      Lab 5
 * Filename:        Filter.hpp
 *
 * Description:     Abstract class for a file filter that transforms the text
 *                  of a file and writes it to an output file.
 ************************************************************************/
#ifndef FILTER_HPP
#define FILTER_HPP

#include <fstream>

class Filter
{
private:
    virtual char transform(char) = 0;
public:
    virtual void doFilter(std::ifstream &, std::ofstream &);
};

#endif

/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/4/2015
 * Last Modified:   2/6/2015
 * Assignment:      Lab 5
 * Filename:        CopyFilter.hpp
 *
 * Description:     Represents a file filter that does nothing to the text of
 *                  a file and outputs it unchanged.
 ************************************************************************/
#ifndef COPY_FILTER_HPP
#define COPY_FILTER_HPP

#include "Filter.hpp"

class CopyFilter : public Filter
{
private:
    virtual char transform(char);
public:
    // default constructor
    CopyFilter() {}
};

#endif

/*************************************************************************
 * Author:          David Rigert
 * Date Created:    2/4/2015
 * Last Modified:   2/6/2015
 * Assignment:      Lab 5
 * Filename:        UppercaseFilter.hpp
 *
 * Description:     Represents a file filter that transforms the text of
 *                  a file into uppercase.
 *                  Defines the CipherTextFilter class as a friend.
 ************************************************************************/
#ifndef UPPERCASE_FILTER_HPP
#define UPPERCASE_FILTER_HPP

#include "Filter.hpp"

class UppercaseFilter : public Filter
{
private:
    virtual char transform(char);
public:
    UppercaseFilter() {}
    friend class CipherTextFilter;
};

#endif

#include "Filter.hpp"
void Filter::doFilter(std::ifstream &in, std::ofstream &out)
{
    char buffer;
    while (in && out)
    {
        buffer = in.get();
        
        if (buffer != EOF)
        {
            buffer = this->transform(buffer);
            out.put(buffer);
        }
    }
}

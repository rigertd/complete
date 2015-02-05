#include "CipherTextFilter.hpp"
#include "UppercaseFilter.hpp"

char CipherTextFilter::transform(char c)
{
    UppercaseFilter uf;
    char buffer = uf.transform(c);
    buffer = this->EncryptionFilter::transform(buffer);
    return buffer;
}

void CipherTextFilter::doFilter(std::ifstream &in, std::ofstream &out)
{
    char buffer;
    int count = 0;
    while (in && out)
    {
        buffer = in.get();
        
        if (buffer != EOF)
        {
            if (
                (buffer >= 'a' && buffer <= 'z') ||
                (buffer >= 'A' && buffer <= 'Z')
                )
            {
                buffer = this->transform(buffer);
                out.put(buffer);
                if (++count == 5)
                {
                    out.put(' ');
                    count = 0;
                }
            }
        }
    }
}

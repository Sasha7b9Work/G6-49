#include "defines.h"
#include "Utils.h"
#pragma warning(push, 0)
#include <iomanip>
#include <sstream>
#include <cstring>
#pragma warning(pop)


char *Utils::DoubleToString(double value)
{
    static char result[100];

    std::stringstream stream;
    stream << std::fixed << std::setprecision(5) << value;

    strcpy_s(result, 90, stream.str().c_str());

    char *p = result;

    while(*p)
    {
        if(*p == '.')
        {
            *p = ',';
        }
        p++;
    }

    return result;
}


double Utils::StringToDouble(pchar value)
{
    char buffer[100];

    strcpy_s(buffer, 90, value);

    char *p = buffer;

    while(*p)
    {
        if(*p == '.')
        {
            *p = ',';
        }
        p++;
    }

    return std::atof(buffer); //-V2508
}

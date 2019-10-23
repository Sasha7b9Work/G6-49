#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "StringUtils.h"
#include "Utils/Math.h"
#include "structs.h"
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <ctype.h>
#endif



char *Float2String(float value)
{
    static char result[100];
    sprintf(result, "%f", value);
    return result;
}


char *Buffer2FloatString(const uint8 *buffer)
{
    return Float2String(Buffer2Float(buffer));
}


float Buffer2Float(const uint8 *buffer)
{
    return BitSet32(buffer).floatValue;
}


char *SU::ToUpper(void *s, uint size)
{
    char *str = reinterpret_cast<char *>(s);

    for(uint i = 0; i < size; i++)
    {
        str[i] = std::toupper(str[i]);
    }

    return str;
}


bool SU::EqualsStrings(uint8 *str1, char *str2, uint size)
{
    return EqualsStrings(static_cast<void *>(str1), static_cast<void *>(str2), size);
}


bool SU::EqualsStrings(void *s1, void *s2, uint size)
{
    char *str1 = static_cast<char *>(s1);
    char *str2 = static_cast<char *>(s2);

    for (uint i = 0; i < size; i++)
    {
        if (str1[i] != str2[i])
        {
            return false;
        }
    }
    return true;
}

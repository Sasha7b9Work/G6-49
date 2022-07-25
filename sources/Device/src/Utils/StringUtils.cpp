// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "StringUtils.h"
#include "Utils/Math.h"
#include "structs.h"
#include <cstdio>


char *SU::Float2String(float value)
{
    static char result[100];
    std::sprintf(result, "%f", value);
    return result;
}


char *SU::Buffer2FloatString(const uint8 *buffer)
{
    return Float2String(Buffer2Float(buffer));
}


float SU::Buffer2Float(const uint8 *buffer)
{
    return BitSet32(buffer).floatValue;
}


char *SU::Bin2StringN(uint value, char buffer[33], int n)
{
    buffer[n] = '\0';

    for (int i = n - 1; i >= 0; i--)
    {
        buffer[i] = (value & 0x01) ? '1' : '0';
        value >>= 1;
    }

    return buffer;
}

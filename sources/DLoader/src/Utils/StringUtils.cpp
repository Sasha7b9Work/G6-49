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

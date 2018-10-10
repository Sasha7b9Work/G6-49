#include "defines.h"
#include "StringUtils.h"
#include "Utils/Math.h"
#include "structs.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char *Float2String(float value)
{
    static char result[100];
    sprintf(result, "%f", value);
    return result;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Buffer2FloatString(const uint8 *buffer)
{
    return Float2String(Buffer2Float(buffer));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float Buffer2Float(const uint8 *buffer)
{
    return BitSet32(buffer).floatValue;
}

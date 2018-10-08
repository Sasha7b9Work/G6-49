#include "defines.h"
#include "StringUtils.h"
#include "Utils/Math.h"
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

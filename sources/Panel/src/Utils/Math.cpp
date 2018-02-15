#include "Math.h"
#include <string.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float MaxFloat(float val1, float val2, float val3)
{
    float retValue = val1;
    if (val2 > retValue)
    {
        retValue = val2;
    }
    if (val3 > retValue)
    {
        retValue = val3;
    }
    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Math_Sign(int value)
{
    if (value > 0)
    {
        return 1;
    }
    if (value < 0)
    {
        return -1;
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void AddLimitationFloat(float *val, float delta, float min, float max)
{
    float sum = *val + delta;
    if (sum < min)
    {
        *val = min;
    }
    else if (sum > max)
    {
        *val = max;
    }
    else
    {
        *val = sum;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int FindSymbol(char *string, char symbol)
{
    for (uint i = 0; i < strlen(string); i++)
    {
        if (string[i] == symbol)
        {
            return (int)i;
        }
    }

    return -1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int NumDigitsInUInt(uint value)
{
    int pow = 10;
    int retValue = 1;

    while (1)
    {
        if ((int)value - pow < 0)
        {
            return retValue;
        }

        pow *= 10;
        retValue++;
    }
}

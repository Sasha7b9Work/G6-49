#include "defines.h"
#include "Math.h"
#include <cmath>
#if __ARMCLIB_VERSION < 6070001
#include <cstdlib>
#include <string.h>
#else
#include <cstdlib>
#include <cstring>
#endif
#include <limits>


float Math::MaxFloat(float val1, float val2, float val3)
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


uint Math::Pow10(int pow)
{
    uint result = 1;

    while (pow--)
    {
        result *= 10;
    }

    return result;
}


int Math::DigitsInIntPart(float value)
{
    float absValue = std::fabsf(value);

    int num = 0;

    do
    {
        ++num;
        absValue /= 10.0F;
    } while (absValue >= 1.0F);

    return num;
}


int Math::DigitsInInt(int value)
{
    if(value < 0)
    {
        value = -value;
    }

    int result = 0;

    do 
    {
        result++;
        value /= 10;
    } while(value > 0);

    return result;
}


float Math::RoundFloat(float value, int numDigits)
{
    float absValue = std::fabsf(value);

    int digsInInt = Math::DigitsInIntPart(absValue);

    if (digsInInt < numDigits)  // Подстрахуемся
    {
        uint pow = Math::Pow10(numDigits - digsInInt);
        absValue = static_cast<int>(absValue * pow + 0.5F) / static_cast<float>(pow);
    }

    return (value > 0.0F) ? absValue : -absValue;
}


bool Math::IsEquals(float x, float y)
{
    return std::fabsf(x - y) < std::numeric_limits<float>::epsilon();
}

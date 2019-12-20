#include "defines.h"
#include "Math.h"
#include "Settings/Settings.h"
#include <cmath>
#if __ARMCLIB_VERSION < 6070001
#include <cstdlib>
#include <string.h>
#else
#include <cstdlib>
#include <cstring>
#endif
#include <limits>


int LowSignedBit(uint value)
{
    int verValue = 1;

    for (int i = 0; i < 32; i++)
    {
        if (verValue & static_cast<int>(value))
        {
            return i;
        }

        verValue <<= 1;
    }

    return -1;
}


bool IntInRange(int value, int min, int max)
{
    return (value >= min) && (value <= max);
}


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


uint Pow10(int pow)
{
    uint result = 1;

    while (pow--)
    {
        result *= 10;
    }

    return result;
}


float RandFloat(float min, float max)
{
    float delta = max - min;
    return min + ((std::rand() / (float)RAND_MAX) * delta);
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


float Math::RoundFloat(float value, int numDigits)
{
    float absValue = std::fabsf(value);

    int digsInInt = Math::DigitsInIntPart(absValue);

    if (digsInInt < numDigits)  // Подстрахуемся
    {
        uint pow = Pow10(numDigits - digsInInt);
        absValue = static_cast<int>(absValue * pow + 0.5F) / static_cast<float>(pow);
    }

    return (value > 0.0F) ? absValue : -absValue;
}


bool Math::IsEquals(float x, float y)
{
    return std::fabsf(x - y) < std::numeric_limits<float>::epsilon();
}


bool FloatsIsEquals(float value0, float value1, float epsilonPart)
{
    float max = std::fabsf(value0) > std::fabsf(value1) ? std::fabsf(value0) : std::fabsf(value1);

    float epsilonAbs = max * epsilonPart;

    return std::fabsf(value0 - value1) < epsilonAbs;
}

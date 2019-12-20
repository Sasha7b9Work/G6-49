#include "defines.h"
#include "Math.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <limits>


int LowSignedBit(uint value)
{
    int verValue = 1;

    for (int i = 0; i < 32; i++)
    {
        if (verValue & (static_cast<int>(value)))
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


int Pow10(int pow)
{
    int retValue = 1;

    while (pow--)
    {
        retValue *= 10;
    }

    return retValue;
}


float RandFloat(float min, float max)
{
    float delta = max - min;
    return min + ((std::rand() / (float)RAND_MAX) * delta);
}


bool IsEquals(float x, float y)
{
    return std::fabsf(x - y) < std::numeric_limits<float>::epsilon();
}


bool FloatsIsEquals(float value0, float value1, float epsilonPart)
{
    float max = std::fabsf(value0) > std::fabsf(value1) ? std::fabsf(value0) : std::fabsf(value1);

    float epsilonAbs = max * epsilonPart;

    return std::fabsf(value0 - value1) < epsilonAbs;
}

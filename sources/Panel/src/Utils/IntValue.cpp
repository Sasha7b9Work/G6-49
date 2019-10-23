#include "defines.h"
#include "IntValue.h"
#include <cstring>
#include "Display/Text.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IntValue::IntValue(const char *buffer, int _posComma) : sign(1)
{
    posComma = _posComma;

    char temp[20];
    std::strcpy(temp, buffer);

    fract1000 = ToFract1000(temp + posComma + 1);

    temp[posComma + 1] = 0;

    whole = ToWhole(temp);
}


void IntValue::Sub5(char *bufferOut, int *_posComma)
{
    if (whole >= 5)
    {
        whole -= 5;
        sign = 1;
    }
    else
    {
        whole -= (fract1000 == 0) ? 5 : 4;
        fract1000 = 10000 - fract1000;
        sign = -1;
    }

    ToString(bufferOut);
    *_posComma = posComma + 1;
}


int IntValue::ToWhole(const char *buffer)
{
    int numDigits = (int)std::strlen(buffer);

    int pow = 1;

    int result = 0;

    for (int i = numDigits - 1; i >= 0; i--)
    {
        result += pow * (buffer[i] & 0x0f);
        pow *= 10;
    }

    return result;
}


int IntValue::ToFract1000(const char *buffer)
{
    int pow = 1000;

    int result = 0;

    uint size = std::strlen(buffer);

    for (uint i = 0; i < size; i++)
    {
        result += pow * (buffer[i] & 0x0f);
        pow /= 10;
    }

    return result;
}


void IntValue::ToString(char *buffer)
{
    buffer[0] = (sign > 0) ? '+' : '-';

    WholeToString(buffer + 1, posComma + 1);
    Fract1000toString(buffer + posComma + 1 + 1, 4 - posComma);
}


void IntValue::WholeToString(char *buffer, int numDigits)
{
    int number = (whole > 0) ? whole : -whole;

    for (int i = numDigits - 1; i >= 0; i--)
    {
        buffer[i] = (number % 10) | 0x30;
        number /= 10;
    }
}


void IntValue::Fract1000toString(char *buffer, int numDigits)
{
    for (int i = 0; i < 4; i++)
    {
        if (i < numDigits)
        {
            buffer[i] = (char)(DigitFromFract1000(i) | 0x30);
        }
    }
}


int IntValue::DigitFromFract1000(int pos)
{
    int number = fract1000;

    for (int i = 3; i > pos; i--)
    {
        number /= 10;
    }

    return number % 10;
}

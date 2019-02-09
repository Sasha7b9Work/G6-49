#include "defines.h"
#include "IntValue.h"
#include <cstring>
#include "Display/Text.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IntValue::IntValue(const char *buffer, int _posComma)
{
    posComma = _posComma;

    char temp[20];
    std::strcpy(temp, buffer);

    fract1000 = ToFract1000(temp + posComma + 1);

    temp[posComma + 1] = 0;

    whole = ToWhole(temp);

    Text::DrawFormatText(100, 100, "Whole = %d", whole);
    Text::DrawFormatText(100, 110, "Fract = %d", fract1000);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void IntValue::Sub(IntValue & /*sub*/, char *bufferOut)
{
    return ToString(bufferOut);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int IntValue::ToWhole(char *buffer)
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int IntValue::ToFract1000(char *buffer)
{
    int pow = 1000;

    int result = 0;

    for (uint i = 0; i < std::strlen(buffer); i++)
    {
        result += pow * (buffer[i] & 0x0f);
        pow /= 10;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void IntValue::ToString(char *buffer)
{
    WholeToString(buffer, posComma + 1);
    Fract1000toString(buffer + posComma + 1, 4 - posComma);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void IntValue::WholeToString(char *buffer, int numDigits)
{
    int number = whole;

    for (int i = numDigits - 1; i >= 0; i--)
    {
        buffer[i] = (number % 10) | 0x30;
        number /= 10;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int IntValue::DigitFromFract1000(int pos)
{
    int number = fract1000;

    for (int i = 3; i > pos; i--)
    {
        number /= 10;
    }

    return number % 10;
}

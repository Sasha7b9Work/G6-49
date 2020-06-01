#include "common/Common.h"
#include "Generator/MathSupport.h"
#include <cstring>


static void AddChar(char *buffer, const Value &value, int pos)
{
    char digit[2] = { 0, 0 };
    digit[0] = MathDoubleValue::GetChar(value, pos);
    std::strcat(buffer, digit);
}


pString Value::ToString(Order::E, bool sign) const
{
    static char buffer[50];

    buffer[0] = '\0';

    if (sign)
    {
        buffer[0] = (Sign() > 0) ? '+' : '-';
        buffer[1] = '\0';
    }

    int first = MathDoubleValue::GetPositionFirstDigit(*this, GetOrder());

    for (int i = first; i >= 0; i--)
    {
        AddChar(buffer, *this, i);
    }

    std::strcat(buffer, ".");

    for (int i = -1; i >= -9; i--)
    {
        AddChar(buffer, *this, i);
    }

    while (buffer[std::strlen(buffer) - 1] == '0')
    {
        buffer[std::strlen(buffer) - 1] = '\0';
    }

    return buffer;
}

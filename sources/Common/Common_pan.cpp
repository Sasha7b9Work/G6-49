#include "common/Common.h"
#include "Generator/MathSupport.h"
#include "Settings/Settings.h"
#include <cstring>


static void AddChar(char *buffer, const Value &value, int pos, Order::E order)
{
    char digit[2] = { 0, 0 };
    digit[0] = MathDouble::GetChar(value, pos, order);
    std::strcat(buffer, digit);
}


pString Value::ToString(bool sign, Order::E order) const
{
    static char buffer[50];

    double valueD = ToDouble();
    valueD = valueD;

    buffer[0] = '\0';

    if (sign)
    {
        buffer[0] = (Sign() > 0) ? '+' : '-';
        buffer[1] = '\0';
    }

    order = (order == Order::Count) ? GetOrder() : order;

    int first = MathDouble::GetPositionFirstDigit(*this, order);

    for (int i = first; i >= 0; i--)
    {
        AddChar(buffer, *this, i, order);
    }

    std::strcat(buffer, ".");

    for (int i = -1; i >= -9; i--)
    {
        AddChar(buffer, *this, i, order);
    }

    while (buffer[std::strlen(buffer) - 1] == '0') //-V1044
    {
        buffer[std::strlen(buffer) - 1] = '\0';
    }

    return buffer;
}


pString Order::Suffix(Order::E order)
{
    static const pString suf[Count][2] =
    {
        {"ּ", "M"},
        {"ך",  "k"},
        {"",   ""},
        {"ל",  "m"},
        {"לך", "u"},
        {"ם",  "n"}
    };

    return suf[order][LANGUAGE];
}


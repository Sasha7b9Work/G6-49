// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "common/Common.h"
#include "Settings/Settings.h"
#include <cstring>


static void AddChar(char *buffer, const Value &value, int pos, Order::E order)
{
    char digit[2] = { 0, 0 };
    digit[0] = value.GetChar(pos, order);
    std::strcat(buffer, digit);
}


cstr Value::ToString(bool sign, Order::E order) const
{
    static char buffer[50];

    buffer[0] = '\0';

    if (sign)
    {
        buffer[0] = (Sign() > 0) ? '+' : '-';
        buffer[1] = '\0';
    }

    order = (order == Order::Count) ? GetOrder() : order;

    int first = GetPositionFirstDigit(order);

    for (int i = first; i >= 0; i--)
    {
        AddChar(buffer, *this, i, order);
    }

    std::strcat(buffer, ".");

    for (int i = -1; i >= -9; i--)
    {
        AddChar(buffer, *this, i, order);
    }

    return buffer;
}


cstr Order::Suffix(Order::E order)
{
    static const cstr suf[Count][2] =
    {
        {"ּ", "M"},
        {"ך",  "k"},
        {"",   ""},
        {"ל",  "m"},
        {"לך", "u"},
        {"ם",  "n"}
    };

    return suf[(int)order][LANGUAGE];
}


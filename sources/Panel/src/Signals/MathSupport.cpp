// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Common.h"
#include "Display/Text.h"
#include "Signals/MathSupport.h"
#include "Signals/Parameters.h"
#include "Utils/StringUtils.h"
#include <cstring>


// Погасить незначащие символы
static void RepayEmptySymbols(char *buffer)
{
    char *first = buffer;

    int sign = 0;

    if (*buffer == '-')
    {
        sign = -1;
        buffer[0] = ' ';
        buffer++;
    }
    else if (*buffer == '+')
    {
        sign = +1;
        buffer[0] = ' ';
        buffer++;
    }

    while (*buffer == '0')
    {
        if (*(buffer + 1) == '.')
        {
            break;
        }
        *buffer = ' ';
        buffer++;
    }

    if (sign != 0)
    {
        *(buffer - 1) = (sign < 0) ? '-' : '+';
    }

    char *end = SU::FindEnd(buffer);

    buffer = end - 1;

    while (*buffer == '0')
    {
        *buffer = '\0';
        buffer--;
    }

    while (*first == ' ') //-V1044
    {
        buffer = first;

        do 
        {
            buffer++;
            *(buffer - 1) = *buffer;
        } while (*buffer != '\0');
    }
}


static cstr ZeroValue(const DParam *param)
{
    TypeDParam::E type = param->GetType();

    if (type == TypeDParam::Amplitude)
    {
        return "00.0000";
    }
    else if (type == TypeDParam::Offset)
    {
        return "+00.0000";
    }

    return "0.0";
}


cstr DParam::GetIndicatedValue() const
{
    static const int NUM_DIGITS = 6;
    static const int LENGTH_BUFFER = NUM_DIGITS + 2;

    static char buffer[LENGTH_BUFFER + 1];                      // Дополнительно завершающий ноль и точка
    buffer[LENGTH_BUFFER - 1] = '\0';

    if (value.Abs() == 0)
    {
        std::strcpy(buffer, ZeroValue(this));
    }
    else
    {
        bool sign = IsSigned();

        Order::E order = IsNotOrdered() ? Order::One : Order::Count;

        if (sign)
        {
            buffer[0] = (value.Sign() > 0) ? '+' : '-';
        }

        int posDigit = GetPositionFirstDigit(order);     // Позиция первого значащего символа относительно точки

        for (int i = sign ? 1 : 0; i < LENGTH_BUFFER - 1; i++)
        {
            char symbol = value.GetChar(posDigit, order);

            buffer[i] = symbol;

            if (posDigit == 0)
            {
                buffer[++i] = '.';
            }

            posDigit--;
        }
    }

    RepayEmptySymbols(buffer);

    if (IsSigned())
    {
        buffer[LENGTH_BUFFER - 1] = ' ';
        buffer[LENGTH_BUFFER] = '\0';
    }

    return buffer;

}


int DParam::GetPositionFirstDigit(Order::E order) const
{
    if (IsPhase())
    {
        return 2;
    }

    return IsNotOrdered() ? 1 : GetValue().GetPositionFirstDigit(order);
}


int Value::GetPositionFirstDigit(Order::E order) const
{
    Value _value = *this;

    _value.SetSign(1);

    int result = 0;

    if (_value.Integer() > 0)
    {
        int whole = _value.Integer();

        while (whole > 9)
        {
            whole /= 10;
            result++;
        }
    }
    else
    {
        int fract = _value.FractNano();

        if (fract == 0)
        {
            return 0;
        }

        do
        {
            result--;
            fract *= 10;
        } while (fract < (100 * 1000 * 1000));
    }

    return result - Order::GetPow10(order == Order::Count ? _value.GetOrder() : order);
}


char Value::GetChar(int position, Order::E order) const
{
    int digit = GetDigit(position, order);

    return (digit == -1) ? '\0' : (char)(GetDigit(position, order) | 0x30);
}


int Value::GetDigit(int position, Order::E order) const
{
    Value _value = *this;
    _value.SetSign(1);

    order = (order == Order::Count) ? _value.GetOrder() : order;

    position += Order::GetPow10(order);

    if(position < 0)
    {
        int divider = 100 * 1000 * 1000;       // На это число будем делить количество наносекунд

        int fract = _value.FractNano();

        while(position < - 1)
        {
            if (divider == 0)
            {
                return -1;
            }

            fract %= divider;
            divider /= 10;
            position++;
        }

        return (divider == 0) ? -1 : (fract / divider);
    }
    else
    {
        int whole = _value.Integer();

        while(position > 0)
        {
            whole /= 10;
            position--;
        }

        return (whole % 10);
    }
}

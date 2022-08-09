// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Common.h"
#include "Display/Text.h"
#include "Signals/MathSupport.h"
#include "Signals/Parameters.h"
#include "Utils/StringUtils.h"
#include <cstring>


namespace MathParameterDouble
{
    // Собственно параметр, по которому будут производится вычислеиня
    static ParameterDouble *param = nullptr;
}


namespace MathDouble
{
    static Order::E CalculateOrder(const ParameterDouble* param)
    {
        return param->IsNotOrdered() ? Order::One : Order::Count;
    }
}


void MathParameterDouble::SetParameterDouble(ParameterDouble* parameter)
{
    param = parameter;
}


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


static cstr ZeroValue(const ParameterDouble *param)
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


cstr MathDouble::GetIndicatedValue(const ParameterDouble *param)
{
    static const int NUM_DIGITS = 6;
    static const int LENGTH_BUFFER = NUM_DIGITS + 2;

    static char buffer[LENGTH_BUFFER + 1];                      // Дополнительно завершающий ноль и точка
    buffer[LENGTH_BUFFER - 1] = '\0';

    Value value = param->GetValue();

    if (value.Abs() == 0)
    {
        std::strcpy(buffer, ZeroValue(param));
    }
    else
    {
        bool sign = param->IsSigned();
        Order::E order = CalculateOrder(param);

        if (sign)
        {
            buffer[0] = (value.Sign() > 0) ? '+' : '-';
        }

        int posDigit = GetPositionFirstDigit(param, order);     // Позиция первого значащего символа относительно точки

        for (int i = sign ? 1 : 0; i < LENGTH_BUFFER - 1; i++)
        {
            char symbol = GetChar(value, posDigit, order);
            buffer[i] = symbol;

            if (posDigit == 0)
            {
                buffer[++i] = '.';
            }

            posDigit--;
        }
    }

    RepayEmptySymbols(buffer);

    if (param->IsSigned())
    {
        buffer[LENGTH_BUFFER - 1] = ' ';
        buffer[LENGTH_BUFFER] = '\0';
    }

    return buffer;

}


int MathDouble::GetPositionFirstDigit(const ParameterDouble *param, Order::E order)
{
    if (param->IsPhase())
    {
        return 2;
    }

    return param->IsNotOrdered() ? 1 : GetPositionFirstDigit(param->GetValue(), order);
}


int MathDouble::GetPositionFirstDigit(const Value &val, Order::E order)
{
    Value value = val;
    value.SetSign(1);

    int result = 0;

    if (value.Integer() > 0)
    {
        int whole = value.Integer();

        while (whole > 9)
        {
            whole /= 10;
            result++;
        }
    }
    else
    {
        int fract = value.FractNano();

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

    return result - Order::GetPow10(order == Order::Count ? value.GetOrder() : order);
}


char MathDouble::GetChar(const Value &value, int position, Order::E order)
{
    int digit = GetDigit(value, position, order);

    return (digit == -1) ? '\0' : static_cast<char>(GetDigit(value, position, order) | 0x30);
}


int MathDouble::GetDigit(const Value &val, int position, Order::E order)
{
    Value value = val;
    value.SetSign(1);

    order = (order == Order::Count) ? value.GetOrder() : order;

    position += Order::GetPow10(order);

    if(position < 0)
    {
        int divider = 100 * 1000 * 1000;       // На это число будем делить количество наносекунд

        int fract = value.FractNano();

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
        int whole = value.Integer();

        while(position > 0)
        {
            whole /= 10;
            position--;
        }

        return (whole % 10);
    }
}


int MathParameterDouble::GetNumberDigitsBeforeComma(Order::E order)
{
    if (param->IsNotOrdered())
    {
        return param->IsPhase() ? 3 : 2;
    }

    return MathDouble::GetPositionFirstDigit(param->GetMax(), order) + 1;
}


int MathParameterDouble::GetNumberDigitsAfterComma(Order::E)
{
    if (param->IsNotOrdered())
    {
        return 3;
    }

    return -Order::GetPow10(param->GetMin().GetOrder()) + Order::GetPow10(param->GetValue().GetOrder());
}


int MathParameterInteger::GetMaxNumberDigits(const ParameterInteger *parameter)
{
    Value max = parameter->GetMax();

    int result = 1;

    while (max.Integer() != 0)
    {
        result++;
        max.Div(10);
    }

    return result - 1;
}

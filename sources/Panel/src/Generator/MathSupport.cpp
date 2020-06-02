#include "defines.h"
#include "common/Common.h"
#include "Display/Text.h"
#include "Generator/MathSupport.h"
#include "Generator/Parameters.h"
#include "Utils/StringUtils.h"
#include <cstring>


ParameterDouble *MathParameterValue::param = nullptr;


// Погасить незначащие символы
static void RepayEmptySymbols(char *buffer)
{
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
        if ((*buffer == '\0') || (*(buffer + 1) == '.'))
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
        *buffer = ' ';
        buffer--;
    }
}


static Order::E CalculateOrder(const ParameterDouble *param)
{
    return param->IsVoltage() ? Order::One : Order::Count;
}


static pString ZeroValue(const ParameterDouble *param)
{
    ParameterDoubleType::E type = param->GetType();

    if (type == ParameterDoubleType::Amplitude)
    {
        return "00.0000";
    }
    else if (type == ParameterDoubleType::Offset)
    {
        return "+00.0000";
    }

    return "";
}


pString MathValue::GetIndicatedValue(const ParameterDouble *param)
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


int MathValue::GetPositionFirstDigit(const ParameterDouble *param, Order::E order)
{
    return GetPositionFirstDigit(param->GetValue(), order);
}


int MathValue::GetPositionFirstDigit(const Value &val, Order::E order)
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


char MathValue::GetChar(const Value &value, int position, Order::E order)
{
    int digit = GetDigit(value, position, order);

    return (digit == -1) ? '\0' : static_cast<char>(GetDigit(value, position, order) | 0x30);
}


int MathValue::GetDigit(const Value &val, int position, Order::E order)
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


int MathParameterValue::GetNumberDigitsBeforeComma(Order::E order)
{
    if (param->IsVoltage())
    {
        return 2;
    }

    return MathValue::GetPositionFirstDigit(param->GetMax(), order) + 1;
}


int MathParameterValue::GetNumberDigitsAfterComma(Order::E order)
{
    if (param->IsVoltage())
    {
        return 3;
    }

    ParameterDoubleType::E type = param->GetType();

    order = (order == Order::Count) ? param->GetValue().GetOrder() : order;

    if (type == ParameterDoubleType::Frequency)
    {
        return 3 + Order::GetPow10(order);
    }

    return 8 + Order::GetPow10(order);
}

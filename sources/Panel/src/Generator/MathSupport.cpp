#include "defines.h"
#include "common/Common.h"
#include "Generator/MathSupport.h"
#include "Generator/Parameters.h"
#include "Utils/StringUtils.h"
#include <cstring>


//FloatValue *LogicFloatValue::value = nullptr;
//ParameterValue *LogicFloatValue::parameter = nullptr;


// Здесь хранится количество значащих цифр перед (индекс 0) и после (индекс 1) запятой. Знак не учитывается
static const int numberDigits[ParameterValueType::Count][2] =
{
    {9,  3},   // Frequency,
    {10, 9},   // Period,
    {2,  3},   // Amplitude,
    {2,  3},   // Offset,
    {10, 9},   // Duration,
    {10, 9},   // DutyRatio,
    {10, 9},   // Phase,
    {10, 9},   // Delay,
    {10, 9},   // DurationRise,
    {10, 9},   // DurationFall,
    {10, 9},   // DurationStady,
    {10, 9},   // DutyFactor,
    {10, 9},   // ManipulationDuration,
    {10, 9},   // ManipulationPeriod,
    {10, 9},   // PacketPeriod,
    {10, 9},   // PacketNumber,
    {10, 9}    // Exit,
};

ParameterValue *MathParameterValue::param = nullptr;


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


static Order::E CalculateOrder(const ParameterValue *param)
{
    if (param->IsVoltage())
    {
        FloatValue value = param->GetValue();

        if (value < FloatValue("1.0") && value > FloatValue("-1.0"))
        {
            return Order::Milli;
        }
    }

    return Order::Count;
}


static pString ZeroValue(const ParameterValue *param)
{
    ParameterValueType::E type = param->GetType();

    if (type == ParameterValueType::Amplitude)
    {
        return "00000.0";
    }
    else if (type == ParameterValueType::Offset)
    {
        return "+00000.0";
    }

    return "";
}


pString MathFloatValue::GetIndicatedValue(const ParameterValue *param)
{
    static const int NUM_DIGITS = 6;
    static const int LENGTH_BUFFER = NUM_DIGITS + 2;

    static char buffer[LENGTH_BUFFER + 1];                      // Дополнительно завершающий ноль и точка
    buffer[LENGTH_BUFFER - 1] = '\0';

    FloatValue value = param->GetValue();

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
            char symbol = GetChar(value, posDigit);
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


static int GetPositionFirstDigitVoltate(const ParameterValue *param, Order::E)
{
    FloatValue value = param->GetValue();

    return (value < FloatValue("1.0") && value > FloatValue("-1.0")) ? 4 : 1;
}


int MathFloatValue::GetPositionFirstDigit(const ParameterValue *param, Order::E order)
{
    if (param->IsVoltage())
    {
        return GetPositionFirstDigitVoltate(param, order);
    }

    FloatValue value = param->GetValue();
    value.SetSign(1);

    CorrectValueOnOrder(&value, order);

    int result = 0;

    if(value.Integer() > 0)
    {
        int whole = value.Integer();        // Целая часть числа

        while(whole > 9)
        {
            whole /= 10;
            result++;
        }
    }
    else
    {
        int fract = value.FractNano();

        if(fract == 0)
        {
            return 0;
        }

        do
        {
            result--;
            fract *= 10;
        } while(fract < (1000 * 1000 * 1000));
    }

    return result;
}


char MathFloatValue::GetChar(const FloatValue &value, int postition, Order::E order)
{
    return static_cast<char>(GetDigit(value, postition, order) | 0x30);
}


void MathFloatValue::CorrectValueOnOrder(FloatValue *value, Order::E order)
{
    if (order == Order::Count)
    {
        order = GetOrder(*value);
    }

    if (order == Order::Mega)
    {
        value->Div(1000 * 1000);
    }
    else if (order == Order::Kilo)
    {
        value->Div(1000);
    }
    else if (order == Order::Milli)
    {
        value->Mul(1000);
    }
    else if (order == Order::Micro)
    {
        value->Mul(1000 * 1000);
    }
    else if (order == Order::Nano)
    {
        value->Mul(1000 * 1000 * 1000);
    }
}


int MathFloatValue::GetDigit(const FloatValue &val, int position, Order::E order)
{
    FloatValue value = val;
    value.SetSign(1);

    CorrectValueOnOrder(&value, order);

    if(position < 0)
    {
        int divider = 100 * 1000 * 1000;       // На это число будем делить количество наносекунд

        int fract = value.FractNano();

        while(position < -1)
        {
            fract %= divider;
            divider /= 10;
            position++;
        }

        return fract / divider;
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


Order::E MathFloatValue::GetOrder(FloatValue value)
{
    value.SetSign(1);

    int integer = value.Integer();

    if (integer >= 1000 * 1000)
    {
        return Order::Mega;
    }
    else if (integer >= 1000)
    {
        return Order::Kilo;
    }
    else if (integer > 0)
    {
        return Order::One;
    }

    int fract = value.FractNano();

    if (fract >= 1000 * 1000)
    {
        return Order::Milli;
    }
    else if (fract >= 1000)
    {
        return Order::Micro;
    }

    return Order::Nano;
}


int MathParameterValue::GetNumberDigitsBeforeComma()
{
    ParameterValueType::E type = param->GetType();

    if (type == ParameterValueType::Frequency)
    {
        int integerPart = param->GetValue().Integer();

        int def = numberDigits[type][0];

        if (integerPart < 1000)
        {
            return def;
        }
        else if (integerPart < 1000 * 1000)
        {
            return def - 3;
        }

        return def - 6;
    }

    return numberDigits[type][0];
}


int MathParameterValue::GetNumberDigitsAfterComma()
{
    ParameterValueType::E type = param->GetType();

    if (type == ParameterValueType::Frequency)
    {
        int integerPart = param->GetValue().Integer();

        int def = numberDigits[type][1];

        if (integerPart < 1000)
        {
            return def;
        }
        else if (integerPart < 1000 * 1000)
        {
            return def + 3;
        }

        return def + 6;
    }

    return numberDigits[param->GetType()][1];
}

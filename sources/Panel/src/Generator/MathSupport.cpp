#include "defines.h"
#include "common/Common.h"
#include "Generator/MathSupport.h"
#include "Generator/Parameters.h"


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


pString MathFloatValue::GetIndicatedValue(const ParameterValue *param)
{
    static const int numDigits = 5;

    FloatValue value = param->GetValue();
    ParameterValueType::E type = param->GetType();
    bool sign = param->IsSigned();

    static char buffer[20];

    buffer[numDigits + (sign ? 2 : 1)] = '\0';

    int position = (type == ParameterValueType::Offset || type == ParameterValueType::Amplitude) ? GetPositionFirstDigit(value) : 1;

    int posComma = PositionComma(position);

    if(sign)
    {
        buffer[0] = value.Sign() == 1 ? '+' : '-';
    }

#define POS(i) ((sign) ? ((i) + (1)) : (i))

    for(int i = 0; i <= numDigits; i++)
    {
        if(i == posComma)
        {
            buffer[POS(i)] = '.';
        }
        else
        {
            buffer[POS(i)] = GetChar(value, position);
            position--;
        }
    }

    return buffer;

}


int MathFloatValue::GetPositionFirstDigit(const FloatValue &_value)
{
    FloatValue value = _value;
    value.SetSign(1);

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


int MathFloatValue::PositionComma(int posFirstDigit)
{
    int result = posFirstDigit - 5;

    while(result < 1)
    {
        result += 3;
    }

    return result;
}


Order::E MathFloatValue::GetOrder(const FloatValue value)
{
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

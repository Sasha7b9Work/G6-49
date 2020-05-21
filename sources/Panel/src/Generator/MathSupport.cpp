#include "defines.h"
#include "common/Common.h"
#include "Generator/MathSupport.h"
#include "Generator/Parameters.h"


//FloatValue *LogicFloatValue::value = nullptr;
//ParameterValue *LogicFloatValue::parameter = nullptr;


// Здесь хранится количество значащих цифр перед (индекс 0) и после (индекс 1) запятой. Знак не учитывается
static const int numberDigits[ParameterValue::Count][2] =
{
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10},
    {10, 10}
};

ParameterValue *MathParameterValue::param = nullptr;


pString MathFloatValue::GetStringValue(const FloatValue &value, bool sign, int numDigits, Order::E *order, int posFirst)
{
    static char buffer[20];

    buffer[numDigits + (sign ? 2 : 1)] = '\0';

    int position = (posFirst == 255) ? GetPositionFirstDigit(value) : posFirst;

    int posComma = PositionComma(position, order);

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


char MathFloatValue::GetChar(const FloatValue &value, int postition)
{
    return static_cast<char>(GetDigit(value, postition) | 0x30);
}


int MathFloatValue::GetDigit(const FloatValue &_value, int position)
{
    FloatValue value = _value;
    value.SetSign(1);

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


int MathFloatValue::PositionComma(int posFirstDigit, Order::E *order)
{
    if(posFirstDigit > 5)
    {
        *order = Order::Mega;
    }
    else if(posFirstDigit > 2)
    {
        *order = Order::Kilo;
    }
    else if(posFirstDigit > -1)
    {
        *order = Order::One;
    }
    else if(posFirstDigit > -4)
    {
        *order = Order::Milli;
    }
    else if(posFirstDigit > -7)
    {
        *order = Order::Micro;
    }
    else
    {
        *order = Order::Nano;
    }

    int result = posFirstDigit - 5;

    while(result < 1)
    {
        result += 3;
    }

    return result;
}


int MathParameterValue::GetNumberDigitsBeforeComma()
{
    return numberDigits[param->Type()][0];
}


int MathParameterValue::GetNumberDigitsAfterComma()
{
    return numberDigits[param->Type()][1];
}


//bool LogicFloatValue::ChangedSign()
//{
//    if(parameter->Type() == ParameterValue::Offset)
//    {
//        value->SetSign(-value->Sign());
//
//        return true;
//    }
//
//    return false;
//}


//void LogicFloatValue::ChangeDigit(int pos, int delta)
//{
//    uint64 nanos = 1000 * 1000 * 1000;      // Это количество наноединиц в 1 единице
//
//    if(pos > 0)
//    {
//        while(pos > 0)
//        {
//            nanos *= 10;
//            pos--;
//        }
//    }
//    else if(pos < 0)
//    {
//        while(pos < 0)
//        {
//            nanos /= 10;
//            pos++;
//        }
//    }
//
//    FloatValue add(0, 0);                   // Это число будем прибавлять
//
//    add.FromUINT64(nanos);
//
//    if(delta < 0)
//    {
//        add.SetSign(-1);
//    }
//
//    value->Add(add);
//
//    if(*value < parameter->min)
//    {
//        *value = parameter->min;
//    }
//    else if(*value > parameter->max)
//    {
//        *value = parameter->max;
//    }
//}

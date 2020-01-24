#include "defines.h"
#include "Display/Font/Font.h"
#include "Generator/Parameters.h"
#include "Generator/ParametersSupport.h"
#include "Generator/Signals.h"
#include "Generator/Wave.h"
#include <cstring>


Parameter *ParameterPainter::parameter = nullptr;

ParameterValue *LogicFloatValue::parameter = nullptr;
FloatValue *LogicFloatValue::value = nullptr;

pString ParameterPainter::DigitsWithSign(int8 *indexes)
{
    if(parameter->IsValue())
    {
        return DigitsWithSignValue(indexes);
    }

    return "";
}


pString ParameterPainter::Units(Language::E lang)
{
    if(parameter->IsValue())
    {
        return UnitsValue(lang);
    }

    return "";
}


pString ParameterPainter::DigitsWithSignValue(int8 *indexes)
{
    ParameterValue *value = static_cast<ParameterValue *>(parameter);

    if(value->Type() == ParameterValue::Offset)
    {
        return DigitsWithSignOffset(indexes);
    }

    return "";
}


pString ParameterPainter::UnitsValue(Language::E lang)
{
    ParameterValue *value = static_cast<ParameterValue *>(parameter);

    if(value->Type() == ParameterValue::Offset || value->Type() == ParameterValue::Amplitude)
    {
        return (lang == Language::RU) ? "В" : "V";
    }

    return "";
}


pString ParameterPainter::DigitsWithSignOffset(int8 *indexes)
{
    //ParameterValue *value = static_cast<ParameterValue *>(parameter);

    FloatValue value = static_cast<ParameterValue *>(parameter)->value;

    const int NUM_SYMBOLS = 1 + 1 + 1 + 3 + 1;

    static char buffer[NUM_SYMBOLS];

    buffer[0] = (value.Sign() < 0) ? '-' : '+';

    buffer[NUM_SYMBOLS - 1] = '\0';

    buffer[1] = MathFloatValue::GetChar(value, 0);
    indexes[1] = 0;

    buffer[2] = ',';

    buffer[3] = MathFloatValue::GetChar(value, -1);
    indexes[3] = -1;

    buffer[4] = MathFloatValue::GetChar(value, -2);
    indexes[4] = -2;

    buffer[5] = MathFloatValue::GetChar(value, -3);
    indexes[5] = -3;

    return buffer;
}


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

#define POS(i) (sign ? (i + 1) : i)

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


int MathFloatValue::GetDigit(const FloatValue &_value, int position)
{
    FloatValue value = _value;
    value.SetSign(1);

    if(position < 0)
    {
        int divider = 100 * 1000 * 1000;       /// На это число будем делить количество наносекунд

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


char MathFloatValue::GetChar(const FloatValue &value, int postition)
{
    return static_cast<char>(GetDigit(value, postition) | 0x30);
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


ParameterPainterSupporting::ParameterPainterSupporting() : parameter(nullptr), x0(0), positionActive(0)
{
    std::memset(buffer, 0, NUM_SYMBOLS);
    std::memset(indexes, 127, NUM_SYMBOLS);
}


void ParameterPainterSupporting::SetParameter(Parameter *param)
{
    std::memset(indexes, 127, NUM_SYMBOLS);

    parameter = param;

    ParameterPainter::SetPatameter(parameter);
    LogicFloatValue::SetParameter(parameter);

    buffer[0] = '\0';
    std::strcpy(buffer, ParameterPainter::DigitsWithSign(indexes));
    std::strcat(buffer, ParameterPainter::Units());

    int length = X(NumSymbols() + 1);

    x0 = Width() / 2 - length / 2 + X0();

    InitPositionActive();
}


void ParameterPainterSupporting::InitPositionActive()
{
    positionActive = static_cast<int>(PositionFirstUnit()) - 1;
}


uint ParameterPainterSupporting::NumSymbols() const
{
    return std::strlen(buffer);
}


int ParameterPainterSupporting::X(uint pos) const
{
    int delta = 2;

    int result = 0;

    for(uint i = 0; i < pos; i++)
    {
        result += WidthDigit() + delta;
    }

    if(pos >= PositionFirstUnit())
    {
        result += 5;
    }

    return x0 + result;
}


char ParameterPainterSupporting::Symbol(uint pos) const
{
    return buffer[pos];
}


uint ParameterPainterSupporting::PositionFirstUnit() const
{
    for(uint i = 0; i < NumSymbols(); i++)
    {
        if(static_cast<uint8>(Symbol(i)) >= static_cast<uint8>('A'))
        {
            return i;
        }
    }

    return static_cast<uint>(-1);
}


int ParameterPainterSupporting::X0() const
{
    return Wave::Graphics::X();
}


int ParameterPainterSupporting::Width() const
{
    return Wave::Graphics::Width() - 1;
}


int ParameterPainterSupporting::Y0() const
{
    return Wave::Graphics::Y(Chan::GetInverse(CURRENT_CHANNEL)) + 1;
}


void ParameterPainterSupporting::SetActiveNext()
{
    for(int i = positionActive + 1; i < NUM_SYMBOLS; i++)
    {
        if(PositionMayBeActived(i))
        {
            positionActive = i;
            return;
        }
    }

    for(int i = 0; i < NUM_SYMBOLS; i++)
    {
        if(PositionMayBeActived(i))
        {
            positionActive = i;
            return;
        }
    }
}


void ParameterPainterSupporting::SetActivePrev()
{
    if(positionActive != 0)
    {
        for(int i = positionActive - 1; i >= 0; i--)
        {
            if(PositionMayBeActived(i))
            {
                positionActive = i;
                return;
            }
        }
    }

    for(int i = NUM_SYMBOLS - 1; i > 0; i--)
    {
        if(PositionMayBeActived(i))
        {
            positionActive = i;
            return;
        }
    }
}


bool ParameterPainterSupporting::PositionMayBeActived(int pos)
{
    return 
        (indexes[pos] != 127) || 
        (pos == 0 && buffer[pos] == '-') || 
        (pos == 0 && buffer[pos] == '+');
}


void ParameterPainterSupporting::IncreaseInCurrentPosition()
{
    if(!ChangedSign())
    {
        LogicFloatValue::ChangeDigit(indexes[positionActive], +1);
    }

    buffer[0] = '\0';
    std::strcpy(buffer, ParameterPainter::DigitsWithSign(indexes));
    std::strcat(buffer, ParameterPainter::Units());

}


void ParameterPainterSupporting::DecreaseInCurrentPosition()
{
    if(!ChangedSign())
    {
        LogicFloatValue::ChangeDigit(indexes[positionActive], -1);
    }

    buffer[0] = '\0';
    std::strcpy(buffer, ParameterPainter::DigitsWithSign(indexes));
    std::strcat(buffer, ParameterPainter::Units());

}


bool ParameterPainterSupporting::ChangedSign()
{
    if(positionActive == 0 && (buffer[0] == '-' || buffer[0] == '+'))
    {
        return LogicFloatValue::ChangedSign();
    }

    return false;
}


void LogicFloatValue::SetParameter(Parameter *param)
{
    if(param->IsValue())
    {
        parameter = static_cast<ParameterValue *>(param);

        value = &parameter->value;
    }
    else
    {
        parameter = nullptr;
        value = nullptr;
    }
}


bool LogicFloatValue::ChangedSign()
{
    if(parameter->Type() == ParameterValue::Offset)
    {
        value->SetSign(-value->Sign());

        return true;
    }

    return false;
}


void LogicFloatValue::ChangeDigit(int pos, int delta)
{
    //FloatValue old = *value;                // Сохраняем значение

    uint64 nanos = 1000 * 1000 * 1000;      // Это количество наноединиц в 1 единице

    if(pos > 0)
    {
        while(pos > 0)
        {
            nanos *= 10;
            pos--;
        }
    }
    else if(pos < 0)
    {
        while(pos < 0)
        {
            nanos /= 10;
            pos++;
        }
    }

    FloatValue add(0, 0);                   // Это число будем прибавлять

    add.FromUINT64(nanos);

    if(delta < 0)
    {
        add.SetSign(-1);
    }

    value->Add(add);
}

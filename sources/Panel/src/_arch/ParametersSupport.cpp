#include "defines.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Display/Text.h"
#include "Display/Font/Font.h"
#include "Display/WaveGraphics.h"
#include "Generator/Parameters.h"
#include "Generator/ParametersSupport.h"
#include "Generator/Signals.h"
#include "Generator/Wave.h"
#include <cstring>


typedef void (*f)();

struct StructSupport
{
    f func;
    pString u[2];
};


static const StructSupport support[ParameterValue::Count] =
{
    { ParameterPainter::DigitsFrequency,     {"Гц", "Hz"} },
    { ParameterPainter::DigitsPeriod,        { "с", "s" } },
    { ParameterPainter::DigitsAmplitude,     { "В", "V" } },
    { ParameterPainter::DigitsOffset,        { "В", "V" } },
    { ParameterPainter::DigitsDuration,      { "с", "s" } },
    { ParameterPainter::DigitsDutyRatio,     { "с", "s" } },
    { ParameterPainter::DigitsPhase,         { "с", "s" } },
    { ParameterPainter::DigitsDelay,         { "с", "s" } },
    { ParameterPainter::DigitsDurationRise,  { "с", "s" } },
    { ParameterPainter::DigitsDurationFail,  { "с", "s" } },
    { ParameterPainter::DigitsDurationStady, { "с", "s" } },
    { ParameterPainter::DigitsDutyFactor,    { "с", "s" } },
    { ParameterPainter::DigitsManipDuration, { "с", "s" } },
    { ParameterPainter::DigitsManipPeriod,   { "с", "s" } },
    { ParameterPainter::DigitsPacketPeriod,  { "с", "s" } },
    { ParameterPainter::DigitsPacketNumber,  { "с", "s" } },
    { ParameterPainter::DigitsEmpty,         { "с", "s" } }
};


ParameterValue *ParameterPainter::parameter = nullptr;
FloatValue     *ParameterPainter::value = nullptr;
char            ParameterPainter::buffer[ParameterPainter::SIZE_BUFFER];
int8           *ParameterPainter::indexes = nullptr;

ParameterValue *LogicFloatValue::parameter = nullptr;
FloatValue *LogicFloatValue::value = nullptr;


void ParameterPainter::SetPatameter(Parameter *param)
{
    if(param->IsValue())
    {
        parameter = static_cast<ParameterValue *>(param);
        value = &parameter->value;
    }
}


pString ParameterPainter::Digits(int8 *ind)
{
    indexes = ind;

    std::memset(buffer, 0, SIZE_BUFFER);

    buffer[0] = '1';

    support[parameter->Type()].func();

    return buffer;
}


pString ParameterPainter::Units(uint lang)
{
    return support[parameter->Type()].u[lang];
}


void ParameterPainter::DigitsFrequency()
{
    SetChars(0, 7, 8);
    buffer[8] = ',';
    SetChars(9, -1, 6);
}


void ParameterPainter::DigitsTime()
{
    SetChars(0, 5, 6);
    buffer[6] = ',';
    SetChars(7, -1, 8);
}


void ParameterPainter::DigitsPeriod()
{
    DigitsTime();
}


void ParameterPainter::DigitsAmplitude()
{
    SetChars(0, 1, 2);
    buffer[2] = ',';
    SetChars(3, -1, 3);
}


void ParameterPainter::DigitsOffset()
{
    buffer[0] = (value->Sign() < 0) ? '-' : '+';
    SetChar(1, 0);
    buffer[2] = ',';
    SetChars(3, -1, 3);
}


void ParameterPainter::DigitsDuration()
{
    DigitsTime();
}


void ParameterPainter::DigitsDutyRatio()
{
}


void ParameterPainter::DigitsPhase()
{
}

void ParameterPainter::DigitsDelay()
{
    DigitsTime();
}


void ParameterPainter::DigitsDurationRise()
{
    DigitsTime();
}


void ParameterPainter::DigitsDurationFail()
{
    DigitsTime();
}


void ParameterPainter::DigitsDurationStady()
{
    DigitsTime();
}


void ParameterPainter::DigitsDutyFactor()
{
}


void ParameterPainter::DigitsManipDuration()
{
    DigitsTime();
}


void ParameterPainter::DigitsManipPeriod()
{
    DigitsTime();
}


void ParameterPainter::DigitsPacketPeriod()
{
    DigitsTime();
}


void ParameterPainter::DigitsPacketNumber()
{
}


void ParameterPainter::DigitsEmpty()
{
}


void ParameterPainter::SetChar(int posBuffer, int8 indexDigit)
{
    buffer[posBuffer] = MathFloatValue::GetChar(*value, indexDigit);
    indexes[posBuffer] = indexDigit;
}


void ParameterPainter::SetChars(int posBuffer, int8 indexDigit, int8 numDigits)
{
    for(int8 i = 0; i < numDigits; i++)
    {
        SetChar(posBuffer + i, indexDigit - i);
    }
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
    std::strcpy(buffer, ParameterPainter::Digits(indexes));
    std::strcat(buffer, ParameterPainter::Units(LANGUAGE));

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
    return WaveGraphics::X();
}


int ParameterPainterSupporting::Width() const
{
    return WaveGraphics::Width() - 1;
}


int ParameterPainterSupporting::Y0() const
{
    return WaveGraphics::Y(Chan::GetInverse(set.current)) + 1;
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
    std::strcpy(buffer, ParameterPainter::Digits(indexes));
    std::strcat(buffer, ParameterPainter::Units(LANGUAGE));

    PGenerator::SetParameter(parameter);
}


void ParameterPainterSupporting::DecreaseInCurrentPosition()
{
    if(!ChangedSign())
    {
        LogicFloatValue::ChangeDigit(indexes[positionActive], -1);
    }

    buffer[0] = '\0';
    std::strcpy(buffer, ParameterPainter::Digits(indexes));
    std::strcat(buffer, ParameterPainter::Units(LANGUAGE));

    PGenerator::SetParameter(parameter);
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

    if(*value < parameter->min)
    {
        *value = parameter->min;
    }
    else if(*value > parameter->max)
    {
        *value = parameter->max;
    }
}


void ParameterTuner::SetParameter(Parameter *parameter)
{
    support.SetParameter(parameter);
}


Parameter *ParameterTuner::GetParameter()
{
    return support.parameter;
}


bool ParameterTuner::ProcessControl(const Key &key)
{
    if(key.value == Key::RegLeft)
    {
        support.DecreaseInCurrentPosition();
        return true;
    }
    else if(key.value == Key::RegRight)
    {
        support.IncreaseInCurrentPosition();
        return true;
    }
    else if(key.IsUp())
    {
        if(key.value == Key::Left)
        {
            support.SetActivePrev();
            return true;
        }
        else if(key.value == Key::Right)
        {
            support.SetActiveNext();
            return true;
        }
    }

    return false;
}


void ParameterTuner::Draw()
{
    Font::Store();
    Font::Set(TypeFont::_GOSTB20);
    Text::SetUpperCase(false);

    int height = WaveGraphics::Height() - 4;

    Painter::FillRegion(support.X0(), support.Y0(), support.Width(), height, Color::BLACK);

    DrawNameParameter();

    DrawParameter();

    DrawHighlighter();

    Font::Restore();
}


void ParameterTuner::DrawNameParameter()
{
    int length = Font::GetLengthText(support.parameter->Name(LANGUAGE));

    int pos = support.X0() + support.Width() / 2 - length / 2;

    Text::Draw(pos, support.Y0() + 15, support.parameter->Name(LANGUAGE), Color::WHITE);
}


void ParameterTuner::DrawParameter()
{
    Color::WHITE.SetAsCurrent();

    char buffer[2] = { 0, 0 };

    int y = support.Y0() + 60;

    for(uint i = 0; i < support.NumSymbols(); i++)
    {
        buffer[0] = support.Symbol(i);
        int x = support.X(i);
        Painter::FillRegion(x, y, support.WidthDigit() - 1, support.HeightDigit(), Color::GRAY_10);
        Text::Draw(x, y, buffer, Color::WHITE);
    }

}


void ParameterTuner::DrawHighlighter()
{
    int x = support.X(support.GetPositionActive());

    Font::Set(TypeFont::_8);

    Text::Draw4SymbolsInRect(x, support.Y0() + 50, Ideograph::_8::FillDown, Color::GRAY_75);
    Text::Draw4SymbolsInRect(x, support.Y0() + 77, Ideograph::_8::FillUp);
}

#include "defines.h"
#include "Generator/MathSupport.h"
#include "Generator/Tuner.h"
#include "Keyboard/Controls.h"
#include "Settings/Settings.h"
#include <cstring>


typedef void (ParameterPainter::*f)();

struct StructSupport
{
    f func;
    pString u[2];
};


static const StructSupport support[ParameterValue::Count] =
{
    { &ParameterPainter::DigitsFrequency,     {"Ãö", "Hz"} },
    { &ParameterPainter::DigitsPeriod,        { "ñ", "s" } },
    { &ParameterPainter::DigitsAmplitude,     { "Â", "V" } },
    { &ParameterPainter::DigitsOffset,        { "Â", "V" } },
    { &ParameterPainter::DigitsDuration,      { "ñ", "s" } },
    { &ParameterPainter::DigitsDutyRatio,     { "ñ", "s" } },
    { &ParameterPainter::DigitsPhase,         { "ñ", "s" } },
    { &ParameterPainter::DigitsDelay,         { "ñ", "s" } },
    { &ParameterPainter::DigitsDurationRise,  { "ñ", "s" } },
    { &ParameterPainter::DigitsDurationFail,  { "ñ", "s" } },
    { &ParameterPainter::DigitsDurationStady, { "ñ", "s" } },
    { &ParameterPainter::DigitsDutyFactor,    { "ñ", "s" } },
    { &ParameterPainter::DigitsManipDuration, { "ñ", "s" } },
    { &ParameterPainter::DigitsManipPeriod,   { "ñ", "s" } },
    { &ParameterPainter::DigitsPacketPeriod,  { "ñ", "s" } },
    { &ParameterPainter::DigitsPacketNumber,  { "ñ", "s" } },
    { &ParameterPainter::DigitsEmpty,         { "ñ", "s" } }
};


pString ParameterPainter::Digits(int8 *ind)
{
    indexes = ind;

    std::memset(buffer, 0, SIZE_BUFFER);

    buffer[0] = '1';

    (this->*support[parameter->Type()].func)();

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


void ParameterPainter::SetChars(int posBuffer, int8 indexDigit, int8 numDigits)
{
    for(int8 i = 0; i < numDigits; i++)
    {
        SetChar(posBuffer + i, indexDigit - i);
    }
}


void ParameterPainter::SetChar(int posBuffer, int8 indexDigit)
{
    buffer[posBuffer] = MathFloatValue::GetChar(*value, indexDigit);
    indexes[posBuffer] = indexDigit;
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


void ParameterPainter::DigitsTime()
{
    SetChars(0, 5, 6);
    buffer[6] = ',';
    SetChars(7, -1, 8);
}


Tuner::Tuner(Parameter *param) : parameter(param), painter(reinterpret_cast<ParameterValue *>(param))
{

}


bool Tuner::ProcessControl(const Key &key)
{
    if(key.value == Key::RegLeft)
    {
        DecreaseInCurrentPosition();
        return true;
    }
    else if(key.value == Key::RegRight)
    {
        IncreaseInCurrentPosition();
        return true;
    }
    else if(key.IsUp())
    {
        if(key.value == Key::Left)
        {
            SetActivePrev();
            return true;
        }
        else if(key.value == Key::Right)
        {
            SetActiveNext();
            return true;
        }
    }

    return false;
}


void Tuner::DecreaseInCurrentPosition()
{
    if(!ChangedSign())
    {
        LogicFloatValue::ChangeDigit(indexes[positionActive], -1);
    }

    buffer[0] = '\0';
    std::strcpy(buffer, painter.Digits(indexes));
    std::strcat(buffer, painter.Units(LANGUAGE));

    PGenerator::SetParameter(parameter);
}


void Tuner::IncreaseInCurrentPosition()
{
    if(!ChangedSign())
    {
        LogicFloatValue::ChangeDigit(indexes[positionActive], +1);
    }

    buffer[0] = '\0';
    std::strcpy(buffer, painter.Digits(indexes));
    std::strcat(buffer, painter.Units(LANGUAGE));

    PGenerator::SetParameter(parameter);
}


void Tuner::SetActivePrev()
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


void Tuner::SetActiveNext()
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


bool Tuner::ChangedSign()
{
    if(positionActive == 0 && (buffer[0] == '-' || buffer[0] == '+'))
    {
        return LogicFloatValue::ChangedSign();
    }

    return false;
}


bool Tuner::PositionMayBeActived(int pos)
{
    return
        (indexes[pos] != 127) ||
        (pos == 0 && buffer[pos] == '-') ||
        (pos == 0 && buffer[pos] == '+');
}

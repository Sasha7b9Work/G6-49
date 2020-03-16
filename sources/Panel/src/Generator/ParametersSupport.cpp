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


ParameterValue *ParameterPainter::parameter = nullptr;
FloatValue     *ParameterPainter::value = nullptr;
char            ParameterPainter::buffer[ParameterPainter::SIZE_BUFFER];
int8           *ParameterPainter::indexes = nullptr;


void ParameterPainter::SetPatameter(Parameter *param)
{
    if(param->IsValue())
    {
        parameter = static_cast<ParameterValue *>(param);
        value = &parameter->value;
    }
}


void ParameterPainter::DigitsTime()
{
    SetChars(0, 5, 6);
    buffer[6] = ',';
    SetChars(7, -1, 8);
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
    return WaveGraphics::Y(Chan::GetInverse(CURRENT_CHANNEL)) + 1;
}


bool ParameterPainterSupporting::PositionMayBeActived(int pos)
{
    return 
        (indexes[pos] != 127) || 
        (pos == 0 && buffer[pos] == '-') || 
        (pos == 0 && buffer[pos] == '+');
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





void ParameterTuner::SetParameter(Parameter *parameter)
{
    support.SetParameter(parameter);
}


Parameter *ParameterTuner::GetParameter()
{
    return support.parameter;
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

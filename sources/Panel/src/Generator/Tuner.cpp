#include "defines.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Display/Text.h"
#include "Display/WaveGraphics.h"
#include "Generator/MathSupport.h"
#include "Generator/Parameters.h"
#include "Generator/Tuner.h"
#include "Generator/Wave.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


using namespace Primitives;


bool Digit::Increase()
{
    if (value < '9')
    {
        value++;
        return true;
    }

    return false;
}


bool Digit::Decrease()
{
    if (value > '0')
    {
        value--;
        return true;
    }

    return false;
}


Indicator::Indicator() : indexHighlight(0)
{
    digits[MAX_NUM_DIGITS - 1] = '\0';

    digits[5] = '.';
}


void Indicator::Draw(int x, int y)
{
    static const int dx = 12;

    Font::Store();

    Font::Set(TypeFont::_GOSTB20);

    int pos = 0;

    while (digits[pos] != '\0')
    {
        Char(digits[pos]).Draw(x, y);

        if (pos == indexHighlight)
        {
            HighlightSymbol(x, y);
        }

        x += dx;

        if (CommaInPosition(pos))
        {
            x -= 7;
        }

        pos++;
    }

    Font::Restore();
}


void Indicator::HighlightSymbol(int x, int y)
{
    Font::Store();

    Font::Set(TypeFont::_7);

    x += 2;

    Char(Ideograph::_7::FillDown).Draw(x, y - 7);

    Char(Ideograph::_7::FillUp).Draw(x, y + 19);

    Font::Restore();
}


void Indicator::HighlightLeft()
{
    do
    {
        Math::CircleDecrease(&indexHighlight, 0, LastDigit());
    } while (CommaInPosition(indexHighlight));
}


void Indicator::HighlightRight()
{
    do
    {
        Math::CircleIncrease(&indexHighlight, 0, LastDigit());
    } while (CommaInPosition(indexHighlight));
}


void Indicator::HighlightIncrease()
{
    digits[indexHighlight].Increase();
}


void Indicator::HighlightDecrease()
{
    digits[indexHighlight].Decrease();
}


int Indicator::LastDigit()
{
    for (int i = 0; i < MAX_NUM_DIGITS; i++)
    {
        if (digits[i] == '\0')
        {
            return (i - 1);
        }
    }

    return (MAX_NUM_DIGITS - 1);
}


bool Indicator::CommaInPosition(int pos)
{
    return (digits[pos] == '.');
}


void TunerDisplay::Draw()
{
    Chan ch = param->GetForm()->GetWave()->GetChannel();

    int x = WaveGraphics::X();
    int y = WaveGraphics::Y(ch.GetInverse());

    Rectangle(WaveGraphics::Width(), WaveGraphics::Height()).DrawFilled(x, y, Color::BLUE_10, Color::WHITE);

    DrawTitle(x, y, WaveGraphics::Width());

    DrawValue(x, y + 50);
}


void TunerDisplay::DrawTitle(int x, int y, int width)
{
    Font::Store();

    Font::Set(TypeFont::_GOSTB20);

    Text(param->Name(LANGUAGE)).DrawInCenterRect(x, y, width, 30, Color::WHITE);

    Font::Restore();
}


void TunerDisplay::DrawValue(int x, int y)
{
//    Order::E order = Order::One;
//
//    pString str = MathFloatValue::GetStringValue(param->value, param->IsSigned(), 10, &order);

    indicator.Draw(x + 20, y);
}


bool TunerDisplay::OnControlKey(const Control control)
{
    if (control.key == Key::Left)
    {
        indicator.HighlightLeft();
        return true;
    }
    else if (control.key == Key::Right)
    {
        indicator.HighlightRight();
        return true;
    }
    else if (control.key == Key::RotateLeft)
    {
        indicator.HighlightDecrease();
        return true;
    }
    else if (control.key == Key::RotateRight)
    {
        indicator.HighlightIncrease();
        return true;
    }

    return false;
}


Tuner::Tuner(ParameterValue *_param) : param(_param), display(param)
{

}


void Tuner::Draw()
{
    display.Draw();
}


bool Tuner::OnControlKey(const Control control)
{
    if (control.IsCursors() || control.IsRotate())
    {
        return display.OnControlKey(control);
    }

    return false;
}

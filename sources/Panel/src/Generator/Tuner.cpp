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

    digits[5] = Digit::COMMA;
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


bool Indicator::OnControlKey(const Control control) //-V801
{
    if (control.IsRotate() || control.IsCursors())
    {
        if (control.Is(Key::Left))
        {
            HighlightToLeft();
        }
        else if (control.Is(Key::Right))
        {
            HighlightToRight();
        }
        else if (control.Is(Key::RotateLeft))
        {
            DecreaseInPosition(indexHighlight);
        }
        else if (control.Is(Key::RotateRight))
        {
            IncreaseInPosition(indexHighlight);
        }

        return true;
    }

    return false;
}


void Indicator::IncreaseInPosition(int pos)
{
    if (!digits[pos].Increase())
    {
        int left = FindPositionLeftDigit(pos);

        if (left >= 0)
        {
            if (CanBeIncreased(left))
            {
                digits[pos].Set('0');
                IncreaseInPosition(left);
            }
        }
    }
}


void Indicator::DecreaseInPosition(int pos)
{
    if (!digits[pos].Decrease())
    {
        int left = FindPositionLeftDigit(pos);

        if (left >= 0)
        {
            if (CanBeDecreased(left))
            {
                digits[pos].Set('9');
                DecreaseInPosition(left);
            }
        }
    }
}


void Indicator::HighlightToLeft()
{
    do
    {
        Math::CircleDecrease(&indexHighlight, 0, LastDigit());
    } while (CommaInPosition(indexHighlight));

}


void Indicator::HighlightToRight()
{
    do
    {
        Math::CircleIncrease(&indexHighlight, 0, LastDigit());
    } while (CommaInPosition(indexHighlight));
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
    return (digits[pos] == Digit::COMMA);
}


bool Indicator::FirstSignedDigitInPosition(int pos)
{
    for (int i = 0; digits[i] != '\0'; i++)
    {
        if (digits[i].IsDigit())
        {
            return (i == pos);
        }
    }

    return false;
}


int Indicator::FindPositionLeftDigit(int pos)
{
    if (FirstSignedDigitInPosition(pos))
    {
        return -1;
    }

    do 
    {
        pos--;
    } while (!digits[pos].IsDigit());

    return pos;
}


bool Indicator::CanBeDecreased(int pos)
{
    if (digits[pos].IsDigit())
    {
        if (digits[pos] > '0')
        {
            return true;
        }
    }

    int left = FindPositionLeftDigit(pos);

    if (left < 0)
    {
        return false;
    }

    return CanBeDecreased(left);
}


bool Indicator::CanBeIncreased(int pos)
{
    if (FirstSignedDigitInPosition(pos) && digits[pos] == '9')
    {
        return false;
    }

    return true;
}


TunerDisplay::TunerDisplay(Tuner *_tuner) : tuner(_tuner)
{
}


void TunerDisplay::Draw()
{
    Chan ch = tuner->Parameter()->GetForm()->GetWave()->GetChannel();

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

    Text(tuner->Parameter()->Name(LANGUAGE)).DrawInCenterRect(x, y, width, 30, Color::WHITE);

    Font::Restore();
}


void TunerDisplay::DrawValue(int x, int y)
{
    indicator.Draw(x + 20, y);
}


bool TunerDisplay::OnControlKey(const Control control) //-V801
{
    return indicator.OnControlKey(control);
}


void TunerDisplay::Init()
{
    MathParameterValue::SetParameterValue(tuner->Parameter());

    int before = MathParameterValue::GetNumberDigitsBeforeComma();
    int after = MathParameterValue::GetNumberDigitsAfterComma();

    for (int i = 0; i < before + after + 1; i++)
    {
        indicator.Digits()[i].Set('0');
    }

    indicator.Digits()[before].Set(Digit::COMMA);

    indicator.Digits()[before + after + 1] = '\0';

    const FloatValue &value = tuner->Parameter()->value;

    int pos = before - 1;                               // –азр€д в этой позиции будем заполн€ть значени€ми целых

    for (int i = 0; i < before; i++)
    {
        indicator.Digits()[pos].Set(MathFloatValue::GetChar(value, i));
        pos--;
    }

    pos = before + 1;                                   // “еперь в эту позицию будем записывать рразр€ды после зап€той

    for (int i = 0; i < after; i++)
    {
        indicator.Digits()[pos].Set(MathFloatValue::GetChar(value, -i - 1));
        pos++;
    }
}


Tuner::Tuner(ParameterValue *_param) : param(_param), display(this)
{
}


void Tuner::Init()
{
    display.Init();
}


void Tuner::Draw()
{
    display.Draw();
}


bool Tuner::OnControlKey(const Control control) //-V801
{
    if (control.IsCursors() || control.IsRotate())
    {
        return display.OnControlKey(control);
    }

    return false;
}

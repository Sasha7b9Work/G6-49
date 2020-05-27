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


Indicator::Indicator(TunerDisplay *_display) : indexHighlight(0), display(_display)
{
    digits[MAX_NUM_DIGITS - 1] = '\0';

    digits[5] = Digit::COMMA;
}


int Indicator::Draw(int x, int y)
{
    static const int dx = 12;

    int pos = 0;

    bool fullFill = false;        // Если true, то цифры и надо выводить полным цветом

    while (digits[pos] != '\0')
    {
        Color color = CalculateColor(pos, &fullFill);

        x += AdditionShiftForDigit(pos);

        Char(digits[pos]).Draw(x, y, color);

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

    return x;
}


Color Indicator::CalculateColor(int pos, bool *fullFill)
{
    Color color = Color::WHITE;

    if (digits[pos].IsNumber())
    {
        if (PositionComma() == (pos + 1))
        {
            *fullFill = true;
        }

        if (!*fullFill)
        {
            if (digits[pos] != '0')
            {
                *fullFill = true;
            }
        }

        if (!*fullFill)
        {
            color = Color::GRAY_50;
        }
    }

    return color;
}


int Indicator::AdditionShiftForDigit(int pos)
{
    static const int d = 5;

    int posComma = PositionComma();

    for (int i = 1; i < 5; i++)
    {
        if (pos == (posComma - i * 3))
        {
            return d;
        }
        if (pos == (posComma + i * 3 + 1))
        {
            return d;
        }
    }

    return 0;
}


void Indicator::HighlightSymbol(int x, int y)
{
    Font::StoreAndSet(TypeFont::_7);

    x += 2;

    Char(Ideograph::_7::FillDown).Draw(x, y - 7, Color::WHITE);

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


bool Indicator::IsSigned()
{
    return !digits[0].IsNumber();
}


void Indicator::IncreaseInPosition(int pos)
{
    ParameterValue *param = display->GetTuner()->GetParameter();

    FloatValue value = param->GetValue();

    if (!ChangeSign(&value, pos))
    {
        FloatValue step = StepPosition(pos);

        value.Add(step);

        LIMITATION_ABOVE(value, param->GetMax());
    }

    display->Init(value);
}


void Indicator::DecreaseInPosition(int pos)
{
    ParameterValue *param = display->GetTuner()->GetParameter();

    FloatValue value = param->GetValue();

    double valueF = value.ToDouble();

    if (!ChangeSign(&value, pos))
    {
        FloatValue step = StepPosition(pos);

        double stepF = step.ToDouble();
        stepF = stepF;

        value.Sub(step);

        valueF = value.ToDouble();

        LIMITATION_BELOW(value, param->GetMin());

        valueF = value.ToDouble();

        int integer = value.Integer();
        integer = integer;

        int fract = value.FractNano();
        fract = fract;
        fract = fract;
    }

    display->Init(value);
}


bool Indicator::ChangeSign(FloatValue *value, int pos)
{
    if (IsSigned() && (pos == 0))
    {
        if (value->Abs() != 0)
        {
            value->SetSign(-value->Sign());
        }

        return true;
    }

    return false;
}


FloatValue TunerDisplay::GetValue() const
{
    return FloatValue(indicator.GetStringDigits());
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


int Indicator::PositionComma()
{
    for (int i = 0; i < MAX_NUM_DIGITS; i++)
    {
        if (digits[i] == Digit::COMMA)
        {
            return i;
        }
    }

    return -1;
}


bool Indicator::FirstSignedDigitInPosition(int pos)
{
    for (int i = 0; digits[i] != '\0'; i++)
    {
        if (digits[i].IsNumber())
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
    } while (!digits[pos].IsNumber());

    return pos;
}


bool Indicator::CanBeDecreased(int pos)
{
    if (digits[pos].IsNumber())
    {
        if (digits[pos] > '0')
        {
            return true;
        }
    }
    else
    {
        return true;
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


char *Indicator::GetStringDigits() const
{
    static char result[MAX_NUM_DIGITS];

    int i = -1;

    do 
    {
        i++;
        result[i] = digits[i];
    } while (result[i] != '\0');

    return result;
}


FloatValue Indicator::StepPosition(int pos)
{
    int posAboutComma = pos - PositionComma();          // Позиция разряда относительно точки

    if (posAboutComma < 0)
    {
        posAboutComma++;
    }

    FloatValue step("1.0");

    Order::E order = MathFloatValue::GetOrder(display->GetTuner()->GetParameter()->GetValue());

    step.MulPow10(Order::GetPow10(order) - posAboutComma);
     
    return step;
}


void Indicator::InitHighlight()
{
    indexHighlight = PositionComma() - 1;
}


TunerDisplay::TunerDisplay(Tuner *_tuner) : tuner(_tuner), indicator(this)
{
}


void TunerDisplay::Draw()
{
    Chan ch = tuner->GetParameter()->GetForm()->GetWave()->GetChannel();

    int x = WaveGraphics::X();
    int y = WaveGraphics::Y(ch.GetInverse());

    Rectangle(WaveGraphics::Width(), WaveGraphics::Height()).DrawFilled(x, y, Color::BLUE_10, Color::WHITE);

    Font::StoreAndSet(TypeFont::_GOSTB20);

    DrawTitle(x, y, WaveGraphics::Width());

    x = DrawValue(x, y + 50);

    DrawUnits(x, y + 50);

    Font::Restore();
}


void TunerDisplay::DrawTitle(int x, int y, int width)
{
    Text(tuner->GetParameter()->Name(LANGUAGE)).DrawInCenterRect(x, y, width, 30, Color::WHITE);
}


void TunerDisplay::DrawUnits(int x, int y)
{
    Text::SetUpperCase(false);

    Text(tuner->GetParameter()->GetUnits(LANGUAGE)).Draw(x + 7, y);

    Text::SetUpperCase(true);
}


int TunerDisplay::DrawValue(int x, int y)
{
    return indicator.Draw(x + 20, y);
}


bool TunerDisplay::OnControlKey(const Control control) //-V801
{
    return indicator.OnControlKey(control);
}


void TunerDisplay::Init()
{
    MathParameterValue::SetParameterValue(tuner->GetParameter());

    for (int i = 0; i < Indicator::MAX_NUM_DIGITS; i++)
    {
        indicator.digits[i].Set('\0');
    }

    int before = MathParameterValue::GetNumberDigitsBeforeComma();

    indicator.digits[before].Set(Digit::COMMA);

    FillDigitsIntegerPart();

    FillDigitsFractPart();
}


void TunerDisplay::FillDigitsIntegerPart()
{
    int before = MathParameterValue::GetNumberDigitsBeforeComma();
    ParameterValue *param = tuner->GetParameter();
    FloatValue value = param->GetValue();

    int pos = before - 1;                               // Разряд в этой позиции будем заполнять значениями целых

    if (param->IsSigned())
    {
        indicator.digits[0].Set((value < FloatValue("0.0")) ? '-' : '+');
    }

    for (int i = 0; i < before; i++)
    {
        indicator.digits[pos].Set(MathFloatValue::GetChar(value, i));
        pos--;

        if (param->IsSigned() && (pos == 0))
        {
            break;
        }
    }
}


void TunerDisplay::FillDigitsFractPart()
{
    int before = MathParameterValue::GetNumberDigitsBeforeComma();
    int after = MathParameterValue::GetNumberDigitsAfterComma();
    FloatValue value = tuner->GetParameter()->GetValue();

    int pos = before + 1;                                   // Теперь в эту позицию будем записывать рразряды после запятой

    for (int i = 0; i < after; i++)
    {
        indicator.digits[pos].Set(MathFloatValue::GetChar(value, -i - 1));
        pos++;
    }
}


void TunerDisplay::Init(FloatValue value)
{
    tuner->GetParameter()->SetAndLoadValue(value);
    Init();
}


Tuner::Tuner(ParameterValue *_param) : param(_param), display(this)
{
}


void Tuner::Init()
{
    display.Init();

    display.indicator.InitHighlight();
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

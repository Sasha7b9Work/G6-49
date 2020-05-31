#include "defines.h"
#include "common/Common.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Display/Text.h"
#include "Display/Font/Font.h"
#include "Display/WaveGraphics.h"
#include "Generator/Parameters.h"
#include "Generator/Tuner.h"
#include "Generator/Wave.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


using namespace Primitives;


Tuner::ModeTuning::E Tuner::mode = ModeTuning::Correction;

DisplayEntering::Cursor      DisplayEntering::cursor;
DisplayEntering::EnterBuffer DisplayEntering::buffer;
Order::E                     DisplayEntering::order = Order::Count;

Tuner *Tuner::current = nullptr;


void DisplayEntering::EnterBuffer::Push(const Control &control)
{
    if (stack.Size() > 14)
    {
        return;
    }

    Key::E key = control.key;

    if ((key == Key::Minus) && (Tuner::Current()->GetParameter()->GetType() != ParameterValueType::Offset))
    {
        return;
    }

    if ((key == Key::Minus) && (stack.Size() != 0))
    {
        return;
    }

    if ((key == Key::Dot) && ConsistComma())
    {
        return;
    }

    stack.Push(Key(control.key).ToChar());
}


int DisplayEntering::EnterBuffer::Size() const
{
    return stack.Size();
}


char DisplayEntering::EnterBuffer::At(const int i) const
{
    return stack[i];
}


void DisplayEntering::EnterBuffer::Prepare(ParameterValue *parameter)
{
    stack.Clear();
    param = parameter;
}


String DisplayEntering::EnterBuffer::GetString() const
{
    String string;

    for (int i = 0; i < stack.Size(); i++)
    {
        string.Append(stack[i]);
    }

    return string;
}


bool DisplayEntering::EnterBuffer::ConsistComma() const
{
    for (int i = 0; i < stack.Size(); i++)
    {
        if (stack[i] == Digit::COMMA)
        {
            return true;
        }
    }

    return false;
}


void DisplayEntering::Cursor::Init()
{
    timeInit = TIME_MS;
}


void DisplayEntering::Cursor::Draw(int x, int y)
{
    uint time = TIME_MS - timeInit;

    if ((time % 1000) < 500)
    {
        Primitives::Rectangle(10, 2).Fill(x, y);
    }
}


Indicator::Indicator(DisplayCorrection *_display) : indexHighlight(0), display(_display)
{
}


int Indicator::Draw(int x, int y)
{
    static const int dx = 12;

    int pos = 0;

    while (digits[pos] != '\0')
    {
        Color color = CalculateColor(pos);

        x += AdditionShiftForDigit(pos);

        Char(digits[pos]).Draw(x, y, color);

        if (pos == IndexHighlightReal())
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


Color Indicator::CalculateColor(int pos)
{
    if (!digits[pos].IsNumber())
    {
        return Color::FILL;
    }

    if (digits[pos] == '0')
    {
        if (AllNumbersOfLeftIsZero(pos))
        {
            return Color::GRAY_25;
        }
        if (AllNumberOfRightIsZero(pos) && (PositionComma() < pos))
        {
            return Color::GRAY_25;
        }
    }
    return Color::FILL;
}


bool Indicator::AllNumbersOfLeftIsZero(int pos)
{
    for (int i = 0; i < pos; i++)
    {
        if (digits[i].IsNumber() && digits[i] != '0')
        {
            return false;
        }
    }

    return true;
}


bool Indicator::AllNumberOfRightIsZero(int pos)
{
    for (int i = pos + 1; (i < MAX_NUM_DIGITS) && !digits[i].IsEmpty(); i++)
    {
        if (digits[i].IsNumber() && digits[i] != '0')
        {
            return false;
        }
    }

    return true;
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


int Indicator::IndexHighlightReal()
{
    return (indexHighlight < PositionComma()) ? indexHighlight : (indexHighlight + 1);
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
            DecreaseInPosition(IndexHighlightReal());
        }
        else if (control.Is(Key::RotateRight))
        {
            IncreaseInPosition(IndexHighlightReal());
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
    ParameterValue *param = Tuner::Current()->GetParameter();

    DoubleValue value = param->GetValue();

    if (!ChangeSign(&value, pos))
    {
        DoubleValue step = StepPosition(pos);

        value.Add(step);
    }

    if (value != param->GetValue() && value <= param->GetMax())
    {
        display->Init(value);
    }

    if (value > param->GetMax())
    {
        static int counter = 0;
        Display::ShowWarning(String("ѕревышение максимального значени€ %d %s", counter++, param->ToString(param->GetMax())));
    }
}


void Indicator::DecreaseInPosition(int pos)
{
    ParameterValue *param = Tuner::Current()->GetParameter();

    DoubleValue value = param->GetValue();

    if (!ChangeSign(&value, pos))
    {
        DoubleValue step = StepPosition(pos);

        value.Sub(step);
    }

    if (value != param->GetValue() && value >= param->GetMin())
    {
        display->Init(value);
    }
}


bool Indicator::ChangeSign(DoubleValue *value, int pos)
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


void Indicator::HighlightToLeft()
{
    do
    {
        Math::CircleDecrease(&indexHighlight, 0, NumberHighligthingDigits());
    } while (CommaInPosition(IndexHighlightReal()));

}


void Indicator::HighlightToRight()
{
    do
    {
        Math::CircleIncrease(&indexHighlight, 0, NumberHighligthingDigits());
    } while (CommaInPosition(IndexHighlightReal()));
}


int Indicator::NumberHighligthingDigits()
{
    int result = 0;

    for (int i = 0; !digits[i].IsEmpty(); i++)
    {
        if (digits[i].IsNumber() || digits[i].IsSigned())
        {
            result++;
        }
    }

    return result;
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


DoubleValue Indicator::StepPosition(int pos)
{
    int posAboutComma = pos - PositionComma();          // ѕозици€ разр€да относительно точки

    if (posAboutComma < 0)
    {
        posAboutComma++;
    }

    DoubleValue step("1.0");

    Order::E order = Tuner::Current()->GetParameter()->GetValue().GetOrder();

    step.MulPow10(Order::GetPow10(order) - posAboutComma);
     
    return step;
}


void Indicator::InitHighlight()
{
    indexHighlight = PositionComma() - 1;
}


DisplayCorrection::DisplayCorrection(Tuner *_tuner) : tuner(_tuner), indicator(this)
{
}


void DisplayCorrection::Draw()
{
    Chan ch = tuner->GetParameter()->GetForm()->GetWave()->GetChannel();

    int x = WaveGraphics::X();
    int y = WaveGraphics::Y(ch.GetInverse());

    Primitives::Rectangle(WaveGraphics::Width(), WaveGraphics::Height()).DrawFilled(x, y, Color::BLACK, Color::WHITE);

    Font::StoreAndSet(TypeFont::_GOSTB20);

    DrawTitle(x, y, WaveGraphics::Width());

    if (Tuner::InModeEntering())
    {
        DisplayEntering::Draw(x - 5, y + 50, WaveGraphics::Width());
    }
    else
    {
        x = DrawValue(x, y + 50);

        DrawUnits(x, y + 50);
    }

    Font::Restore();
}


void DisplayCorrection::DrawTitle(int x, int y, int width)
{
    String(tuner->GetParameter()->Name()).DrawInCenterRect(x, y, width, 30, Color::WHITE);
}


void DisplayCorrection::DrawUnits(int x, int y)
{
    Font::SetUpperCase(false);

    String(tuner->GetParameter()->GetUnits()).Draw(x + 7, y, Color::WHITE);

    Font::SetUpperCase(true);
}


int DisplayCorrection::DrawValue(int x, int y)
{
    return indicator.Draw(x + 20, y);
}


bool DisplayCorrection::OnControlKey(const Control &control)
{
    if (control.IsEntering())
    {
        return DisplayEntering::OnEnteringKey(control);
    }

    return indicator.OnControlKey(control);
}


void DisplayEntering::Init()
{
    buffer.Prepare(Tuner::Current()->GetParameter());
}


bool DisplayEntering::OnEnteringKey(const Control &control)
{
    if (control.IsEntering())
    {
        if (Tuner::Current()->InModeCorrection())
        {
            Tuner::Current()->SetModeEntering();
            order = Tuner::Current()->GetParameter()->GetValue().GetOrder();
        }

        buffer.Push(control);
        cursor.Init();

        return true;
    }

    return false;
}


void DisplayEntering::Draw(int x, int y, int width)
{
    Color::BLACK.SetAsCurrent();

    int end = DrawValue(x, y);

    Color::WHITE.SetAsCurrent();

    int length = end - x;

    int xCursor = DrawValue(x + (width - length) / 2 + 5, y);

    cursor.Draw(xCursor, y + Font::GetHeight());

    DrawUnits(x, y + 30, width);
}


int DisplayEntering::DrawValue(int x, int y)
{
    for (int i = 0; i < buffer.Size(); i++)
    {
        Char(buffer.At(i)).Draw(x, y);

        x += (buffer.At(i) == Digit::COMMA) ? 9 : 13;
    }

    return x;
}


void DisplayEntering::DrawUnits(int x, int y, int width)
{
    char units[10];

    std::strcpy(units, Order::Suffix(order));
    std::strcat(units, Tuner::Current()->GetParameter()->GetMainUnits());

    Font::SetUpperCase(false);

    String(units).DrawInArea(x, y, width);
}


void DisplayEntering::OnButtonOrderMore()
{
    if (order > Order::Mega)
    {
        order = static_cast<Order::E>(order - 1);
    }
}


void DisplayEntering::OnButtonOrderLess()
{
    if (order < Order::Nano)
    {
        order = static_cast<Order::E>(order + 1);
    }
}


void DisplayCorrection::Init()
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


void DisplayCorrection::FillDigitsIntegerPart()
{
    int before = MathParameterValue::GetNumberDigitsBeforeComma();
    ParameterValue *param = tuner->GetParameter();
    DoubleValue value = param->GetValue();

    int pos = before - 1;                               // –азр€д в этой позиции будем заполн€ть значени€ми целых

    if (param->IsSigned())
    {
        indicator.digits[0].Set((value < DoubleValue("0.0")) ? '-' : '+');
    }

    for (int i = 0; i < before; i++)
    {
        indicator.digits[pos].Set(MathDoubleValue::GetChar(value, i));
        pos--;

        if (param->IsSigned() && (pos == 0))
        {
            break;
        }
    }
}


void DisplayCorrection::FillDigitsFractPart()
{
    int before = MathParameterValue::GetNumberDigitsBeforeComma();
    int after = MathParameterValue::GetNumberDigitsAfterComma();
    DoubleValue value = tuner->GetParameter()->GetValue();

    int pos = before + 1;                                   // “еперь в эту позицию будем записывать рразр€ды после зап€той

    for (int i = 0; i < after; i++)
    {
        indicator.digits[pos].Set(MathDoubleValue::GetChar(value, -i - 1));
        pos++;
    }
}


void DisplayCorrection::Init(DoubleValue value)
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

    current = this;
}


void Tuner::Draw()
{
    display.Draw();
}


bool Tuner::OnControlKey(const Control control) //-V801
{
    if (control.IsCursors() || control.IsRotate() || control.IsEntering())
    {
        return display.OnControlKey(control);
    }

    return false;
}


void Tuner::OnButtonCancel()
{
    PageTuneParameter::CallbackOnButtonCancel();
}


void Tuner::OnButtonApply()
{
    PageTuneParameter::CallbackOnButtonApply();
}


void Tuner::SetModeEntering()
{
    PageTuneParameter::SetModeEntering();
    mode = ModeTuning::Entering;
    DisplayEntering::Init();
}

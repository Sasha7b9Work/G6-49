// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Common.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Display/Text.h"
#include "Signals/Tuner.h"
#include "Display/Font/Font.h"
#include "Display/WaveGraphics.h"
#include "Signals/Parameters.h"
#include "Signals/Wave.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


using namespace Primitives;


Tuner::ModeTuning::E Tuner::mode = ModeTuning::Correction;

Tuner *Tuner::current = nullptr;


namespace DisplayEntering
{
    static Cursor cursor;                   // Мигающий курсор для режима непосредственного ввода

    static EnterBuffer buffer;              // Здесь будем хранить нажатые кнопки в режиме ввода

    static Order::E order = Order::Count;   // Текущий порядок вводимого значения

    static bool noVerifiForValid = true;    // Не проверять на валидность величины значения

    static int DrawValue(int x, int y);

    static void DrawUnits(int x, int y, int width);

    // Попытка добавить в буфер новый символ, соотвествующий нажатой кнопке
    static void TryToAddSymbol(Key::E key);

    // Хранимое значение находится в границах, ограниченных минимальным и максимальным значениями DParam
    static bool ValueInBoundaries();
}


void DisplayEntering::EnterBuffer::Push(Key::E key)
{
    DParam *p = Tuner::Current()->ToDouble();

    if (p && p->IsNotOrdered() && (NumDigitsAfterComma() == 3))
    {
        return;
    }

    if ((key == Key::Minus) && !Tuner::Current()->IsOffset()) { return; }

    if (stack.Size() > 14)                                             { return; }
                                                                       
    if ((key == Key::Minus) && (stack.Size() != 0))                    { return; }
                                                                       
    if ((key == Key::Comma) && ConsistComma())                         { return; }
                                                                       
    if (stack.Size() == 0 && key == Key::Comma)
    {
        stack.Push('0');
        stack.Push('.');
        return;
    }

    stack.Push(Key(key).ToChar());

    if (stack.Size() == 1)
    {
        order = Order::One;
    }
}


void DisplayEntering::EnterBuffer::Pop()
{
    stack.Pop();
}


int DisplayEntering::EnterBuffer::Size() const
{
    return stack.Size();
}


char DisplayEntering::EnterBuffer::At(const int i) const
{
    return stack[i];
}


void DisplayEntering::EnterBuffer::Prepare(DParam *parameter)
{
    stack.Clear();
    param = parameter;
}


String DisplayEntering::EnterBuffer::ToString() const
{
    String string;
    
    int size = stack.Size();

    for (int i = 0; i < size; i++)
    {
        char symbol = stack[i];
        
        string.Append(symbol);
    }

    return string;
}


bool DisplayEntering::EnterBuffer::IsEmpty() const
{
    return stack.IsEmpty() || (stack[0] == '-') || (stack[0] == '+');
}


Value DisplayEntering::ToValue()
{
    Value result(buffer.ToString().c_str(), order);

    return result;
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


int DisplayEntering::EnterBuffer::NumDigitsAfterComma() const
{
    int result = 0;

    for (int i = PosComma() + 1; i < stack.Size(); i++)
    {
        result++;
    }

    return result;
}


int DisplayEntering::EnterBuffer::PosComma() const
{
    for (int i = 0; i < stack.Size(); i++)
    {
        if (stack[i] == Digit::COMMA)
        {
            return i;
        }
    }

    return -1;
}


void DisplayEntering::Cursor::Init()
{
    timeInit = _TIME_MS;
}


void DisplayEntering::Cursor::Draw(int x, int y)
{
    uint time = _TIME_MS - timeInit;

    if ((time % 1000) < 500)
    {
        Primitives::Rectangle(10, 2).Fill(x, y);
    }
}


Indicator::Indicator(DisplayCorrection *_display) : indexHighlight(0), display(_display)
{
}


int Indicator::Draw(int x, int y, int width, const cstr units) const
{
    return Tuner::Current()->ToDouble() ? DrawDouble(x, y, width, units) : DrawInteger(x, y, width);
}


int Indicator::DrawDouble(int x, int y, int width, const cstr units) const
{
    if (Tuner::Current()->IsNotOrdered())
    {
        return DrawDouble(x + 90, y, units, false);
    }

    int end = DrawDouble(x, y, units, true);

    x += (width - (end - x)) / 2;

    return DrawDouble(x, y, units, false);
}


int Indicator::DrawDouble(int x, int y, const cstr units, bool test) const
{
    static const int dx = 12;

    int pos = 0;

    while (digits[pos] != '\0')
    {
        Color color = CalculateColorDouble(pos, test);

        x += AdditionShiftForDigit(pos);

        Char(digits[pos]).Draw(x, y, color);

        if (pos == IndexHighlightReal())
        {
            HighlightSymbol(x, y, test ? Color::BACK : Color::FILL);
        }

        x += dx;

        if (CommaInPosition(pos))
        {
            x -= 7;
        }

        pos++;
    }

    Font::ForceUpperCase(false);

    if (units[0] == Ideograph::_8::Degree)
    {
        Font::StoreAndSet(TypeFont::_8);
        Char(Ideograph::_8::BigDegree).Draw4InRect(x + 5, y - 1, test ? Color::BACK : Color::FILL);
        Font::Restore();
    }
    else
    {
        x = String(units).Draw(x + 5, y, test ? Color::BACK : Color::FILL);
    }

    Font::ForceUpperCase(true);

    return x;
}


int Indicator::DrawInteger(int x, int y, bool test) const
{
    static const int dx = 12;

    int pos = 0;

    while (digits[pos] != '\0')
    {
        Color color = CalculateColorInteger(pos, test);

        Char(digits[pos]).Draw(x, y, color);

        if (pos == IndexHighlightReal())
        {
            HighlightSymbol(x, y, test ? Color::BACK : Color::FILL);
        }

        x += dx;

        pos++;
    }

    return x;
}


int Indicator::DrawInteger(int x, int y, int width) const
{
    int end = DrawInteger(x, y, true);

    x += (width - (end - x)) / 2;

    return DrawInteger(x, y, false);
}


#define COLOR_SHADOW Color::GRAY_10;        // Этим цветом выводятся "назначащие знакоместа"


Color Indicator::CalculateColorInteger(int pos, bool test) const
{
    if (test)
    {
        return Color::BACK;
    }

    if (digits[pos] == '0')
    {
        if (AllNumbersOfLeftIsZero(pos))
        {
            return COLOR_SHADOW;
        }
    }

    return Color::FILL;
}


Color Indicator::CalculateColorDouble(int pos, bool test) const
{
    if (test)
    {
        return Color::BACK;
    }

    if (!digits[pos].IsNumber())
    {
        return Color::FILL;
    }

    if (digits[pos] == '0')
    {
        if (AllNumbersOfLeftIsZero(pos))
        {
            if (RightDigitIzComma(pos))
            {
                return Color::FILL;
            }
            else if ((PositionComma() < pos) && !AllNumbersOfLeftIsZero(pos))
            {
                return Color::FILL;
            }
            else if (IndexHighlightReal() >= pos && (PositionComma() < pos))
            {
                return Color::FILL;
            }
            else
            {
                return COLOR_SHADOW;
            }

        }
        if (AllNumberOfRightIsZero(pos))
        {
            if (PositionComma() < pos)
            {
                if (IndexHighlightReal() >= pos)
                {
                    return Color::FILL;
                }
                else
                {
                    return COLOR_SHADOW;
                }
            }
        }
    }

    return Color::FILL;
}


bool Indicator::AllNumbersOfLeftIsZero(int pos) const
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


bool Indicator::RightDigitIzComma(int pos) const
{
    if (pos == Indicator::MAX_NUM_DIGITS - 1)
    {
        return false;
    }

    return (digits[pos + 1] == Digit::COMMA);
}


bool Indicator::AllNumberOfRightIsZero(int pos) const
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


int Indicator::AdditionShiftForDigit(int pos) const
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


void Indicator::HighlightSymbol(int x, int y, const Color &color) const
{
    Font::StoreAndSet(TypeFont::_7);

    x += 2;

    Char(Ideograph::_7::FillDown).Draw(x, y - 7, color);

    Char(Ideograph::_7::FillUp).Draw(x, y + 19);

    Font::Restore();
}


int Indicator::IndexHighlightReal() const
{
    return (indexHighlight < PositionComma()) ? indexHighlight : (indexHighlight + 1);
}


bool Indicator::OnControlKey(const Control &control) //-V801
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


bool Indicator::IsSigned() const
{
    return !digits[0].IsNumber();
}


void Indicator::IncreaseInPosition(int pos)
{
    if (Tuner::Current()->GetParameter()->IsDouble())
    {
        IncreaseInPositionDouble(pos);
    }
    else
    {
        IncreaseInPositionInteger(pos);
    }
}


void Indicator::IncreaseInPositionDouble(int pos)
{
    DParam *param = Tuner::Current()->ToDouble();

    Value value = param->GetValue();

    if (!ChangeSign(&value, pos))
    {
        Value step = StepPosition(pos);

        value.Add(step);
    }

    if (value != param->GetValue() && value <= param->GetMax())
    {
        display->Init(value);
    }

    DisplayCorrection::ShowMessageOutRangIfNeed(param);
}


void Indicator::IncreaseInPositionInteger(int pos)
{
    IParam *param = Tuner::Current()->ToInteger();

    Value value = param->GetValue();

    if (!ChangeSign(&value, pos))
    {
        Value step = StepPosition(pos);

        value.Add(step);
    }

    if (value != param->GetValue() && value <= param->GetMax())
    {
        display->Init(value);
    }

    DisplayCorrection::ShowMessageOutRangIfNeed(param);
}


void Indicator::DecreaseInPosition(int pos)
{
    Param *param = Tuner::Current()->GetParameter();

    Value value = param->GetValue();

    if (!ChangeSign(&value, pos))
    {
        Value step = StepPosition(pos);

        value.Sub(step);
    }

    if (value != param->GetValue())
    {
        display->Init(value);
    }

    DisplayCorrection::ShowMessageOutRangIfNeed(param);
}


bool Indicator::ChangeSign(Value *value, int pos)
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
        Math::CircleDecrease(&indexHighlight, 0, NumberHighligthingDigits() - 1);
    } while (CommaInPosition(IndexHighlightReal()));

}


void Indicator::HighlightToRight()
{
    do
    {
        Math::CircleIncrease(&indexHighlight, 0, NumberHighligthingDigits() - 1);
    } while (CommaInPosition(IndexHighlightReal()));
}


int Indicator::NumberHighligthingDigits() const
{
    int result = 0;

    for (int i = 0; !digits[i].IsEmpty(); i++)
    {
        if (digits[i].IsNumber() || digits[i].IsSigned())
        {
            result++;
        }
    }

    if (Tuner::Current()->GetParameter()->IsDouble())
    {
        DParam *param = Tuner::Current()->ToDouble();

        TypeDParam::E type = param->GetType();

        if (type == TypeDParam::Duration || type == TypeDParam::Period || type == TypeDParam::PeriodPacket)
        {
            if (digits[result + 1].IsEmpty())
            {
                result--;
            }
        }
    }

    return result;
}


bool Indicator::CommaInPosition(int pos) const
{
    return (digits[pos] == Digit::COMMA);
}


int Indicator::PositionComma() const
{
    Param *param = Tuner::Current()->GetParameter();

    if (param->IsDouble())
    {
        for (int i = 0; i < MAX_NUM_DIGITS; i++)
        {
            if (digits[i] == Digit::COMMA)
            {
                return i;
            }
        }
    }
    else if(param->IsInteger())
    {
        for (int i = 0; i < MAX_NUM_DIGITS; i++)
        {
            if (digits[i].IsEmpty())
            {
                return i;
            }
        }
    }

    return -1;
}


bool Indicator::FirstSignedDigitInPosition(int pos) const
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


int Indicator::FindPositionLeftDigit(int pos) const
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


Value Indicator::StepPosition(int pos) const
{
    int posAboutComma = pos - PositionComma();          // Позиция разряда относительно точки

    if (posAboutComma < 0)
    {
        posAboutComma++;
    }

    Value step("1.0", Order::One);

    Order::E order = DisplayCorrection::CalculateOrderForIndication();

    step.MulPow10(Order::GetPow10(order) - posAboutComma);
     
    return step;
}


void Indicator::InitHighlight()
{
    indexHighlight = PositionComma() - 1;

    if (indexHighlight >= NumberHighligthingDigits() - 1)
    {
        indexHighlight--;
    }
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

    DrawTitle(x - 10, y + 10, WaveGraphics::Width());

    y += 60;

    tuner->ToDouble() ? DrawDouble(x, y) : DrawInteger(x, y);

    Font::Restore();
}


void DisplayCorrection::DrawInteger(int x, int y)
{
    if (Tuner::InModeEntering())
    {
        DisplayEntering::Draw(x + 5, y - 10, WaveGraphics::Width());
    }
    else
    {
        indicator.Draw(WaveGraphics::X(), y, WaveGraphics::Width(), "");
    }
}


void DisplayCorrection::DrawDouble(int x, int y)
{
    if (Tuner::InModeEntering())
    {
        DisplayEntering::Draw(x + 5, y - 10, WaveGraphics::Width());
    }
    else
    {
        indicator.Draw(WaveGraphics::X(), y, WaveGraphics::Width(), tuner->ToDouble()->GetUnits(CalculateOrderForIndication()));
    }
}


void DisplayCorrection::DrawTitle(int x, int y, int width)
{
    String(tuner->GetParameter()->Name()).DrawInCenterRect(x, y, width, 30, Color::WHITE);
}


bool DisplayCorrection::OnControlKey(const Control &control)
{
    if (control.IsEntering())
    {
        return DisplayEntering::OnEnteringKey(control);
    }

    return indicator.OnControlKey(control);
}


bool DisplayCorrection::ShowMessageOutRangIfNeed(Param *param)
{
    SMinMax min_max = param->ValueInRange();

    if (!min_max.valid)
    {
        String min = param->ToString(min_max.min);
        char *buffer = min.c_str();

        while (buffer && *buffer == '0')            // Удаляем все нули в начале
        {
            buffer++;
        }

        if (buffer && (*buffer == '.' || *buffer == ','))       // Если первый символ - точка, добавляем один ноль перед точкой
        {
            if (buffer > min.c_str())
            {
                buffer--;
            }
        }

        String max_str = param->ToString(min_max.max, true);

        String min_str = param->ToString(min_max.min, true);

        Display::Warnings::Top::Show(String("%s : границы диапазона %s ... %s", param->Name(), min_str.c_str(), max_str.c_str()),
                                     String("%s : out of range %s ... %s", param->Name(), min_str.c_str(), max_str.c_str()));
    }
    else
    {
        Display::Warnings::Top::Clear();
    }

    return min_max.valid;
}


void DisplayEntering::Init()
{
    noVerifiForValid = true;
    buffer.Prepare(Tuner::Current()->ToDouble());
}


bool DisplayEntering::OnEnteringKey(const Control &control)
{
    if (control.IsEntering())
    {
        if (Tuner::Current()->InModeCorrection())
        {
            Tuner::Current()->SetModeEntering();
            order = Tuner::Current()->GetParameter()->GetValue().GetOrder();

            if (Tuner::Current()->GetParameter()->IsDouble())
            {
                DParam *p = Tuner::Current()->ToDouble();

                if (p->IsNotOrdered())
                {
                    order = Order::One;
                }
            }
        }

        TryToAddSymbol(control.key);

        cursor.Init();

        return true;
    }

    return false;
}


bool DisplayEntering::ValueInBoundaries()
{
    Param *param = Tuner::Current()->GetParameter();

    Value min = param->GetMin();
    Value max = param->GetMax();

    Value value = ToValue();

    if (noVerifiForValid)
    {
        if (value >= min && value <= max)
        {
            noVerifiForValid = false;
        }

        return true;
    }

    bool result = true;

    if (!buffer.IsEmpty())
    {
        result = DisplayCorrection::ShowMessageOutRangIfNeed(param);
    }

    return result;
}


void DisplayEntering::TryToAddSymbol(Key::E key)
{
    buffer.Push(key);

    if (!ValueInBoundaries())
    {
        buffer.Pop();
    }
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
    if (Tuner::Current()->GetParameter()->IsDouble())
    {
        char units[10];

        std::strcpy(units, Order::Suffix(order));
        std::strcat(units, Tuner::Current()->ToDouble()->GetMainUnits());

        Font::ForceUpperCase(false);

        if (Tuner::Current()->GetParameter()->IsDouble() && Tuner::Current()->ToDouble()->IsPhase())
        {
            Font::StoreAndSet(TypeFont::_8);
            Char(Ideograph::_8::BigDegree).Draw4InRect(x + 130, y);
            Font::Restore();
        }
        else
        {
            String(units).DrawInArea(x, y, width);
        }
    }
}


void DisplayEntering::OnButtonOrderMore()
{
    if (order > Order::Mega)
    {
        Order::Store(order);

        order = (Order::E)(order - 1);

        if (!ValueInBoundaries())
        {
            order = Order::Restore();
        }
    }
}


void DisplayEntering::OnButtonOrderLess()
{
    if (order < Order::Nano)
    {
        Order::Store(order);

        order = (Order::E)(order + 1);

        if (!ValueInBoundaries())
        {
            order = Order::Restore();
        }
    }
}


void DisplayCorrection::Init()
{
    for (int i = 0; i < Indicator::MAX_NUM_DIGITS; i++)
    {
        indicator.digits[i].Set('\0');
    }

    tuner->ToDouble() ? InitDouble() : InitInteger();
}


void DisplayCorrection::InitDouble()
{
    int before = tuner->ToDouble()->GetNumberDigitsBeforeComma(CalculateOrderForIndication());

    indicator.digits[before].Set(Digit::COMMA);

    FillDigitsIntegerPartForDouble();

    FillDigitsFractPartForDouble();
}


void DisplayCorrection::InitInteger()
{
    int numDigits = tuner->ToInteger()->GetMaxNumberDigits();

    for (int i = 0; i < numDigits; i++)
    {
        indicator.digits[i].Set('0');
    }

    FillDigitsForInteger();
}


Order::E DisplayCorrection::CalculateOrderForIndication()
{
    if (Tuner::Current()->GetParameter()->IsDouble())
    {
        DParam *param = Tuner::Current()->ToDouble();

        return param->IsNotOrdered() ? Order::One : param->GetValue().GetOrder();
    }

    return Order::One;
}


void DisplayCorrection::FillDigitsIntegerPartForDouble()
{
    Order::E order = CalculateOrderForIndication();

    DParam *param = tuner->ToDouble();

    int before = param->GetNumberDigitsBeforeComma(order);

    Value value = param->GetValue();

    int pos = before - 1;                               // Разряд в этой позиции будем заполнять значениями целых

    if (param->IsSigned())
    {
        indicator.digits[0].Set((value.Sign() < 0) ? '-' : '+');
    }

    for (int i = 0; i < before; i++)
    {
        indicator.digits[pos].Set(value.GetChar(i, order));
        pos--;

        if (param->IsSigned() && (pos == 0))
        {
            break;
        }
    }
}


void DisplayCorrection::FillDigitsForInteger()
{
    IParam *param = tuner->ToInteger();

    Value value = param->GetValue();

    int pos = param->GetMaxNumberDigits() - 1;

    for (int i = 0; pos >= 0; i++)
    {
        indicator.digits[pos].Set(value.GetChar(i, Order::One));
        pos--;
    }
}


void DisplayCorrection::FillDigitsFractPartForDouble()
{
    Order::E order = CalculateOrderForIndication();

    DParam *param = tuner->ToDouble();

    int before = param->GetNumberDigitsBeforeComma(order);
    int after = param->GetNumberDigitsAfterComma(order);

    Value value = param->GetValue();

    int pos = before + 1;                                   // Теперь в эту позицию будем записывать рразряды после запятой

    for (int i = 0; i < after; i++)
    {
        indicator.digits[pos].Set(value.GetChar(-i - 1, order));
        pos++;
    }
}


void DisplayCorrection::Init(Value value)
{
    Param *param = Tuner::Current()->GetParameter();

    Form *form = param->GetForm();

    if (param->IsDouble())
    {
        tuner->ToDouble()->SetAndLoadValue(value);
    }
    else if (param->IsInteger())
    {
        tuner->ToInteger()->SetAndLoadValue(value);
    }

    if (form->value == TypeForm::Packet)
    {
        if (param == form->FindParameter(TypeDParam::Period) ||
            param == form->FindParameter(TypeIParam::PacketNumber) ||
            param == form->FindParameter(TypeDParam::Duration))
        {
            PPeriodPacket *par_per = (PPeriodPacket *)form->FindParameter(TypeDParam::PeriodPacket);

            if (par_per->RecalcualateValue())
            {
                par_per->SetAndLoadValue(par_per->GetValue());
            }
        }
    }

    Init();
}


Tuner::Tuner(Param *_param) : param(_param), display(this)
{
}


void Tuner::Init()
{
    current = this;

    display.Init();

    display.indicator.InitHighlight();
}


void Tuner::Draw()
{
    display.Draw();
}


bool Tuner::OnControlKey(const Control &control)
{
    if (control.IsCursors() || control.IsRotate() || control.IsEntering())
    {
        return display.OnControlKey(control);
    }

    return false;
}


void Tuner::OnButtonCancel()
{
    if (InModeCorrection())
    {
        PageTuneParameter::CallbackOnButtonCancel();
    }
    else
    {
        mode = ModeTuning::Correction;
        PageTuneParameter::ResetModeEntering();
    }
}


void Tuner::OnButtonApply()
{
    if (InModeCorrection())
    {
        PageTuneParameter::CallbackOnButtonApply();
    }
    else
    {
        if (Current()->IsDouble())
        {
            DParam *paramDouble = Current()->ToDouble();

            TypeDParam::E type = paramDouble->GetType();

            if (type == TypeDParam::PeriodPacket)
            {
                PPeriodPacket *pack_period = (PPeriodPacket *)paramDouble;

                if (DisplayEntering::ToValue() < pack_period->CalculateMinValue())
                {
                    Display::Warnings::Center::_Show(String("Параметр не может быть меньше %s сек", pack_period->CalculateMinValue().ToString(false, Order::One)),
                                               String("The parameter cannot be less %s s", pack_period->CalculateMinValue().ToString(false, Order::One)));
                    return;
                }
            }
            else if (type == TypeDParam::Duration || type == TypeDParam::Period)
            {
                int fractNano = DisplayEntering::ToValue().FractNano();

                if ((fractNano % 10) != 0)
                {
                    Display::Warnings::Center::_Show("Параметр должен быть кратен 10 нс", "Param must be a multiple of 10 ns");
                    return;
                }
            }
        }

        Value value = DisplayEntering::ToValue();

        volatile double val_d = value.ToDouble();
        val_d = val_d;

        display.Init(value);
        display.indicator.InitHighlight();
        mode = ModeTuning::Correction;
        PageTuneParameter::ResetModeEntering();
    }
}


void Tuner::SetModeEntering()
{
    PageTuneParameter::SetModeEntering();
    mode = ModeTuning::Entering;
    DisplayEntering::Init();
}


bool Tuner::IsOffset()
{
    DParam *offset = ToDouble();

    return (offset == nullptr) ? false : (offset->GetType() == TypeDParam::Offset);

}


bool Tuner::IsNotOrdered()
{
    DParam *voltage = ToDouble();

    return (voltage == nullptr) ? false : voltage->IsNotOrdered();
}


DParam *Tuner::ToDouble()
{
    return param->IsDouble() ? (DParam *)param : nullptr;
}


IParam *Tuner::ToInteger()
{
    return param->IsInteger() ? (IParam *)param : nullptr;
}

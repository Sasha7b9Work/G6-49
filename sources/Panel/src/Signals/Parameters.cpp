// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/Generator_p.h"
#include "Signals/Parameters.h"
#include "Interface/Messages_p.h"
#include "Signals/Signals.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cmath>
#include <cstdio>


int CParam::choiceModeStartFree = 0;

DParam DParam::empty(TypeDParam::Frequency, Param::EFuncActive, "Пустой", "Empty", Value(1));
IParam IParam::empty(TypeIParam::PacketNumber, "Пустой", "Empty", Value(1.0));
CParam CParam::empty(TypeCParam::Polarity, Param::EFuncActive, "Пустой", "Empty");
CMSParam CMSParam::empty(TypeCMSParam::Manipulation, "Пустой", "Empty", nullptr);

const Value PAmplitudePic::by_default("1", Order::One);
const Value PPeriod::min_impulse("20", Order::Nano);
const Value PFrequency::min_sin("300", Order::Micro);
const Value PFrequency::max_sin("100", Order::Mega);
const Value PFrequency::min_DDS("100", Order::Micro);
const Value PFrequency::max_DDS("10", Order::Mega);

namespace MathSupport
{
    static cstr ZeroValue(const DParam *param)
    {
        TypeDParam::E type = param->GetType();

        if (type == TypeDParam::AmplitudePic)
        {
            return "00.0000";
        }
        else if (type == TypeDParam::Offset)
        {
            return "+00.0000";
        }

        return "0.0";
    }

    // Погасить незначащие символы
    static void RepayEmptySymbols(char *buffer)
    {
        char *first = buffer;

        int sign = 0;

        if (*buffer == '-')
        {
            sign = -1;
            buffer[0] = ' ';
            buffer++;
        }
        else if (*buffer == '+')
        {
            sign = +1;
            buffer[0] = ' ';
            buffer++;
        }

        while (*buffer == '0')
        {
            if (*(buffer + 1) == '.')
            {
                break;
            }
            *buffer = ' ';
            buffer++;
        }

        if (sign != 0)
        {
            *(buffer - 1) = (sign < 0) ? '-' : '+';
        }

        char *end = SU::FindEnd(buffer);

        buffer = end - 1;

        while (*buffer == '0')
        {
            *buffer = '\0';
            buffer--;
        }

        while (*first == ' ') //-V1044
        {
            buffer = first;

            do
            {
                buffer++;
                *(buffer - 1) = *buffer;
            } while (*buffer != '\0');
        }
    }
}


Param::Param(KindParam::E k, pFuncBV funcActive, pchar nRU, pchar nEN) :
    viewer(this), funcOfActive(funcActive), form(nullptr), parent(nullptr), kind(k)
{
    names[0] = nRU;
    names[1] = nEN;
}


void Param::SetForm(Form *f)
{
    form = f;
}


bool Param::IsDouble() const
{
    return (kind == KindParam::Double);
}


bool Param::IsInteger() const
{
    return (kind == KindParam::Integer);
}


bool Param::IsComposite() const
{
    return (kind == KindParam::Composite);
}


bool Param::IsChoice() const
{
    return (kind == KindParam::Choice);
}


cstr Param::Name() const
{
    return names[LANGUAGE];
}


void Param::SetParent(Param *p)
{
    parent = p;
}


Param *Param::GetParent()
{
    return parent;
}

bool Param::IsOpened() const
{
    return (parent != nullptr);
}


Form *Param::GetForm()
{
    return form;
}


void CMSParam::SetForm(Form *f)
{
    Param::SetForm(f);

    for (int i = 0; i < NumParameters(); i++)
    {
        params[i]->SetForm(f);
    }
}


int CMSParam::NumParameters() const
{
    int counter = 0;

    while (params[counter] != nullptr)
    {
        counter++;
    }

    return counter;
}


CParam *CMSParam::FindParameter(TypeCParam::E p)
{
    for(int i = 0; i < NumParameters(); i++)
    {
        Param *param = params[i];

        if(param->IsChoice())
        {
            CParam *choice = (CParam *)param;

            if(choice->GetType() == p)
            {
                return choice;
            }
        }
    }

    return nullptr;
}


DParam *CMSParam::FindParameter(TypeDParam::E p)
{
    for(int i = 0; i < NumParameters(); i++)
    {
        Param *param = params[i];

        if(param->IsDouble())
        {
            DParam *parameter = (DParam *)param;

            if(parameter->GetType() == p)
            {
                return parameter;
            }
        }
    }

    return nullptr;
}


cstr DParam::GetMainUnits() const
{
    static const cstr units[TypeDParam::Count][2] =
    {
        {"Гц",   "Hz"},   // Frequency
        {"с",    "s"},    // Period
        {"В",    "V"},    // Amplitude
        {"В",    "V"},    // Offset
        {"с",    "s"},    // Duration
        {"\x7b", "\x7b"}, // Phase
        {"с",    "s"},    // Delay
        {"с",    "s"},    // ManipulationDuration
        {"с",    "s"},    // ManipulationPeriod
        {"с",    "s"}     // PacketPeriod
    };

    return units[GetType()][LANGUAGE];
}


cstr DParam::GetUnits(Order::E order) const
{
    if (order == Order::Count)
    {
        order = value.GetOrder();
    }

    static char units[10];

    std::strcpy(units, Order::Suffix(order));
    std::strcat(units, GetMainUnits());

    return units;
}


void DParam::LoadNumberImpulsesIfNeed()
{
    IParam &parameter = CURRENT_FORM->FindParameter(TypeIParam::PacketNumber);

    if (parameter.Exist())
    {
        if (type == TypeDParam::Period || type == TypeDParam::Duration)
        {
            parameter.LoadValue();
        }
    }
}


bool DParam::SetAndLoadValue(double val)
{
    if(val < Min().ToDouble() || val > Max().ToDouble())
    {
        return false;
    }

    value.FromDouble(val);

    PGenerator::SetParameterDouble(*this);

    LoadNumberImpulsesIfNeed();

    return true;
}


bool DParam::SetAndLoadValue(Value val)
{
    value = val;

    PGenerator::SetParameterDouble(*this);

    LoadNumberImpulsesIfNeed();

    return true;
}


void DParam::StoreState()
{
    stored.value = value;
}


void DParam::RestoreState()
{
    value = stored.value;
}


bool IParam::SetAndLoadValue(Value val)
{
    if (val < Min() || val > Max())
    {
        return false;
    }

    value = val;

    PGenerator::SetParameterInteger(*this);

    return true;
}

bool IParam::SetAndLoadValue(int val)
{
    if (Value(val) < Min() || Value(val) > Max())
    {
        return false;
    }

    value.FromINT(val);

    PGenerator::SetParameterInteger(*this);

    return true;
}


void IParam::LoadValue()
{
    PGenerator::SetParameterInteger(*this);
}


bool DParam::IsNotOrdered() const
{
    return (type == TypeDParam::AmplitudePic) || (type == TypeDParam::Offset) || (type == TypeDParam::Phase);
}


bool DParam::IsPhase() const
{
    return (type == TypeDParam::Phase);
}


bool DParam::IsTime() const
{
    static const bool isTime[TypeDParam::Count] =
    {
        false,  // Частота
        true,   // Период
        false,  // Амплитуда
        false,  // Смещение
        true,   // Длительность
        false,  // Сдвиг фазы
        true,   // Задержка
        true,   // Длительность импульсов в режиме манипуляции
        true,   // Период следования импульсов в режиме манипуляции
        true    // Период следования пачек импульсов в пакетном режиме
    };

    return isTime[type];
}


String CMSParam::ToString(String &units) const
{
    units.Free();

    if(type == TypeCMSParam::Manipulation)
    {
        static pchar valuesRU[2] =
        {
            "Откл", "Вкл"
        };

        static pchar valuesEN[2] =
        {
            "Off", "On"
        };

        CParam *enabled = ((CMSParam *)this)->FindParameter(TypeCParam::ManipulationEnabled);

        return String(LANG_RU ? valuesRU[enabled->GetChoice()] : valuesEN[enabled->GetChoice()]);
    }

    return String("");
}


String CParam::ToString(String &units) const
{
    units.Free();

    return String(choices[GetChoice() * 2 + LANGUAGE]);
}


int CParam::GetChoice() const 
{
    if (type == TypeCParam::ModeStart)
    {
        if (form->IsDDS())
        {
            return choiceModeStartFree;
        }
    }

    return choice;
}


void CParam::NextChoice()
{
    if (type == TypeCParam::ModeStartStop)
    {
        Math::CircleIncrease(&choice, 0, NumChoices() - 1);

        Message::SetStartStopMode(GetChoice()).Transmit();

        if (GetChoice() == 0)
        {
            PGenerator::TuneChannel(ChB);
        }
        else
        {
            PGenerator::SetParameterDouble(*A::Impulse::delay);
        }
    }
    else if (type == TypeCParam::ModeStart)
    {
        if (form->IsDDS())
        {
            Math::CircleIncrease(&choiceModeStartFree, 0, NumChoices() - 1);
        }
        else
        {
            Math::CircleIncrease(&choice, 0, NumChoices() - 1);
        }

        PGenerator::LoadStartMode(form->GetWave()->GetChannel(), form->IsDDS() ? 0 : 1, GetChoice());
        PGenerator::TuneChannel(form->GetWave()->GetChannel());
    }
    else
    {
        Math::CircleIncrease(&choice, 0, NumChoices() - 1);

        Chan ch = form->GetWave()->GetChannel();

        PGenerator::TuneChannel(ch);
    }
}


bool CParam::SetAndLoadChoice(int ch)
{
    if(ch < 0 || ch >= NumChoices())
    {
        return false;
    }

    choice = ch;

    PGenerator::SetParameterChoice(*this);

    return true;
}


PManipulation::PManipulation(Param **parameters) : CMSParam(TypeCMSParam::Manipulation, "Манип", "Manip", parameters)
{

}


int CParam::NumChoices() const
{
    int count = 0;

    pchar ch = choices[count];

    while (ch != nullptr)
    {
        count++;
        ch = choices[count];
    }

    return count / 2;
}


DParam::DParam(TypeDParam::E t, pFuncBV funcActive, pchar nameRU, pchar const nameEN, const Value &_value) :
    Param(KindParam::Double, funcActive, nameRU, nameEN), tuner(this), type(t),
        value(_value), resetValue(_value), stored(_value)
{
}


DParam::DParam(const DParam &rhs) :
    Param(KindParam::Double, Param::EFuncActive, rhs.names[0], rhs.names[1]), tuner(rhs.tuner), type(rhs.type),
    value(rhs.value), resetValue(rhs.resetValue), stored(rhs.stored)
{
}


DParam &DParam::operator=(const DParam &rhs)
{
    form = rhs.form;
    parent = rhs.parent;
    kind = rhs.kind;
    names[0] = rhs.names[0];
    names[1] = rhs.names[1];

    tuner = rhs.tuner;
    value = rhs.value;
    resetValue = rhs.resetValue;

    stored.value = rhs.stored.value;

    return *this;
}


void DParam::Reset()
{
    SetAndLoadValue(resetValue);
}


int DParam::GetNumberDigitsBeforeComma(Order::E order) const
{
    if (IsNotOrdered())
    {
        return IsPhase() ? 3 : 2;
    }

    return Max().GetPositionFirstDigit(order) + 1;
}


int DParam::GetNumberDigitsAfterComma(Order::E) const
{
    if (IsNotOrdered())
    {
        return 3;
    }

    return -Order::GetPow10(Min().GetOrder()) + Order::GetPow10(GetValue().GetOrder());
}


static Order::E CalculateOrder(const DParam *param)
{
    return param->IsNotOrdered() ? Order::One : param->GetValue().GetOrder();
}


String DParam::ToString(String &units) const
{
    String result(GetIndicatedValue());

    units.Set(TypeConversionString::None, GetUnits(CalculateOrder(this)));

    return result;
}


String DParam::ToString(Value val, bool delete_zeros) const
{
    String result(val.ToString(IsSigned()));
    if (delete_zeros)
    {
        result.RemoveFromEnd('0');
    }
    result.Append(" ");
    result.Append(GetUnits(val.GetOrder()));

    return result;
}


String IParam::ToString(Value val, bool delete_zeros) const
{
    String result(val.ToString(false, Order::One));

    if (delete_zeros)
    {
        result.RemoveFromEnd('0');
    }

    return result;
}


IParam::IParam(TypeIParam::E t, pchar nameRU, pchar nameEN, const Value &_value) :
    Param(KindParam::Integer, Param::EFuncActive, nameRU, nameEN), tuner(this), type(t), value(_value), resetValue(_value)
{
}


String IParam::ToString(String &units) const
{
    units.Free();

    return String("%d", value.Integer());
}


void IParam::OnPressButtonTune()
{
    PageTuneParameter::SetParameter(this);
    Menu::SetAdditionPage(PageTuneParameter::self);
}



void CParam::OnPressButtonTune()
{
    NextChoice();
}


void DParam::OnPressButtonTune()
{
    PageTuneParameter::SetParameter(this);
    Menu::SetAdditionPage(PageTuneParameter::self);
}


void CMSParam::OnPressButtonTune()
{
    form->OpenCompositeParameter();
}


pchar TypeCParam::Name(TypeCParam::E type)
{
    static pchar  const names[TypeCParam::Count] =
    {
        "Polarity",
        "Mode start",
        "Enabled manipulation"
    };

    return names[type];
}


bool PPeriodPacket::RecalcualateValue()
{
    Value min_value = Min();

    if (GetValue() < min_value)
    {
        SetValue(min_value);

        return true;
    }

    return false;
}


int IParam::GetMaxNumberDigits() const
{
    Value _max = Max();

    int result = 1;

    while (_max.Integer() != 0)
    {
        result++;
        _max.Div(10);
    }

    return result - 1;
}


int DParam::GetPositionFirstDigit(Order::E order) const
{
    if (IsPhase())
    {
        return 2;
    }

    return IsNotOrdered() ? 1 : GetValue().GetPositionFirstDigit(order);
}


cstr DParam::GetIndicatedValue() const
{
    static const int NUM_DIGITS = 6;
    static const int LENGTH_BUFFER = NUM_DIGITS + 2;

    static char buffer[LENGTH_BUFFER + 1];                      // Дополнительно завершающий ноль и точка
    buffer[LENGTH_BUFFER - 1] = '\0';

    if (value.Abs() == 0)
    {
        std::strcpy(buffer, MathSupport::ZeroValue(this));
    }
    else
    {
        bool sign = IsSigned();

        Order::E order = IsNotOrdered() ? Order::One : Order::Count;

        if (sign)
        {
            buffer[0] = (value.Sign() > 0) ? '+' : '-';
        }

        int posDigit = GetPositionFirstDigit(order);     // Позиция первого значащего символа относительно точки

        for (int i = sign ? 1 : 0; i < LENGTH_BUFFER - 1; i++)
        {
            char symbol = value.GetChar(posDigit, order);

            buffer[i] = symbol;

            if (posDigit == 0)
            {
                buffer[++i] = '.';
            }

            posDigit--;
        }
    }

    MathSupport::RepayEmptySymbols(buffer);

    if (IsSigned())
    {
        buffer[LENGTH_BUFFER - 1] = ' ';
        buffer[LENGTH_BUFFER] = '\0';
    }

    return buffer;

}


DParam *Param::ToDouble()
{
    return (kind == KindParam::Double) ? (DParam *)this : &DParam::empty;
}


IParam *Param::ToInteger()
{
    return (kind == KindParam::Integer) ? (IParam *)this : &IParam::empty;
}

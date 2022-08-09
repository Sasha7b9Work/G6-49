// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/Generator_p.h"
#include "Signals/MathSupport.h"
#include "Signals/Parameters.h"
#include "Interface/Messages_p.h"
#include "Signals/Signals.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <cmath>
#include <cstdio>


int ParameterChoice::choiceModeStartFree = 0;


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


void ParameterComposite::SetForm(Form *f)
{
    Param::SetForm(f);

    for (int i = 0; i < NumParameters(); i++)
    {
        params[i]->SetForm(f);
    }
}


int ParameterComposite::NumParameters() const
{
    int counter = 0;

    while (params[counter] != nullptr)
    {
        counter++;
    }

    return counter;
}


ParameterChoice *ParameterComposite::FindParameter(ParameterChoiceType::E p)
{
    for(int i = 0; i < NumParameters(); i++)
    {
        Param *param = params[i];

        if(param->IsChoice())
        {
            ParameterChoice *choice = static_cast<ParameterChoice *>(param);

            if(choice->GetType() == p)
            {
                return choice;
            }
        }
    }

    return nullptr;
}


DParam *ParameterComposite::FindParameter(TypeDParam::E p)
{
    for(int i = 0; i < NumParameters(); i++)
    {
        Param *param = params[i];

        if(param->IsDouble())
        {
            DParam *parameter = static_cast<DParam *>(param);

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
        {"",     ""},     // DutyRatio
        {"\x7b", "\x7b"}, // Phase
        {"с",    "s"},    // Delay
        {"с",    "s"},    // DurationRise
        {"с",    "s"},    // DurationFall
        {"с",    "s"},    // DurationStady
        {"",     ""},     // DutyFactor
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
    ParameterInteger *parameter = CURRENT_FORM->FindParameter(ParameterIntegerType::PacketNumber);

    if (parameter)
    {
        if (type == TypeDParam::Period || type == TypeDParam::Duration)
        {
            parameter->LoadValue();
        }
    }
}


bool DParam::SetAndLoadValue(double val)
{
    if(!InRange(val))
    {
        return false;
    }

    value.FromDouble(val);

    PGenerator::SetParameterDouble(this);

    LoadNumberImpulsesIfNeed();

    return true;
}


bool DParam::SetAndLoadValue(Value val)
{
    if (!InRange(val))
    {
        return false;
    }

    value = val;

    PGenerator::SetParameterDouble(this);

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


bool ParameterInteger::SetAndLoadValue(Value val)
{
    if (!InRange(val))
    {
        return false;
    }

    value = val;

    PGenerator::SetParameterInteger(this);

    return true;
}

bool ParameterInteger::SetAndLoadValue(int val)
{
    if (!InRange(Value(val)))
    {
        return false;
    }

    value.FromINT(val);

    PGenerator::SetParameterInteger(this);

    return true;
}


void ParameterInteger::LoadValue()
{
    PGenerator::SetParameterInteger(this);
}


bool DParam::IsNotOrdered() const
{
    return (type == TypeDParam::Amplitude) || (type == TypeDParam::Offset) || (type == TypeDParam::Phase);
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
        false,  // Скважность
        false,  // Сдвиг фазы
        true,   // Задержка
        true,   // Длительность нарастания
        true,   // Длительность спада
        true,   // Длительность установившего значения
        false,  // Коэффициент заполнения
        true,   // Длительность импульсов в режиме манипуляции
        true,   // Период следования импульсов в режиме манипуляции
        true    // Период следования пачек импульсов в пакетном режиме
    };

    return isTime[type];
}


String ParameterComposite::ToString(String &units) const
{
    units.Free();

    if(type == ParameterCompositeType::Manipulation)
    {
        static pchar valuesRU[2] =
        {
            "Откл", "Вкл"
        };

        static pchar valuesEN[2] =
        {
            "Off", "On"
        };

        ParameterChoice *enabled = const_cast<ParameterComposite *>(this)->FindParameter(ParameterChoiceType::ManipulationEnabled);

        return String(LANG_RU ? valuesRU[enabled->GetChoice()] : valuesEN[enabled->GetChoice()]);
    }

    return String("");
}


String ParameterChoice::ToString(String &units) const
{
    units.Free();

    return String(choices[GetChoice() * 2 + LANGUAGE]);
}


int ParameterChoice::GetChoice() const 
{
    if (type == ParameterChoiceType::ModeStart)
    {
        if (form->IsDDS())
        {
            return choiceModeStartFree;
        }
    }

    return choice;
}


void ParameterChoice::NextChoice()
{
    if (type == ParameterChoiceType::ModeStartStop)
    {
        Math::CircleIncrease(&choice, 0, NumChoices() - 1);

        Message::SetStartStopMode(GetChoice()).Transmit();

        if (GetChoice() == 0)
        {
            PGenerator::TuneChannel(ChB);
        }
        else
        {
            PGenerator::SetParameterDouble(A::Impulse::delay);
        }

        return;
    }

    if (type == ParameterChoiceType::ModeStart)
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
    else if (type == ParameterChoiceType::ClockImpulse)
    {
        Message::SetClockImpulse(GetChoice()).Transmit();

        Display::Warnings::Show2Strings(String("100МГц - длит./период 10нс...40сек"), String("1МГц - длит./период 1мкс...4000сек"),
            String("100MHz - dur./period from 10ns...40sec"), String("1MHz - dur. / period 1us...4000sec"));
    }
    else
    {
        Math::CircleIncrease(&choice, 0, NumChoices() - 1);

        Chan ch = form->GetWave()->GetChannel();

        PGenerator::TuneChannel(ch);
    }
}


bool ParameterChoice::SetAndLoadChoice(int ch)
{
    if(ch < 0 || ch >= NumChoices())
    {
        return false;
    }

    choice = ch;

    PGenerator::SetParameterChoice(this);

    return true;
}


ParameterManipulation::ParameterManipulation(Param **parameters) : ParameterComposite(ParameterCompositeType::Manipulation, "Манип", "Manip", parameters)
{

}


bool DParam::InRange(double val) const
{
    return (val >= GetMin().ToDouble()) && (val <= GetMax().ToDouble());
}


bool DParam::InRange(Value val) const
{
    return (val >= GetMin() && val <= GetMax());
}


bool ParameterInteger::InRange(Value val) const
{
    return (val >= min && val <= max);
}


int ParameterChoice::NumChoices() const
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


DParam::DParam(TypeDParam::E t, pFuncBV funcActive, pchar nameRU, pchar const nameEN,
    const Value &_min_,
    const Value &_max,
    pValueInRange _valueInRange,
    const Value &_value) :
    Param(KindParam::Double, funcActive, nameRU, nameEN), tuner(this), type(t), min(_min_), max(_max), valueInRange(_valueInRange),
    value(_value), resetValue(_value), stored(_value)
{
}


DParam::DParam(const DParam &rhs) :
    Param(KindParam::Double, Param::FuncActive, rhs.names[0], rhs.names[1]), tuner(rhs.tuner), type(rhs.type),
    min(rhs.min), max(rhs.max), valueInRange(EValueInRange), value(rhs.value), resetValue(rhs.resetValue), stored(rhs.stored)
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
    min = rhs.min;
    value = rhs.value;
    resetValue = rhs.resetValue;

    max = rhs.max;

    stored.value = rhs.stored.value;

    return *this;
}


void DParam::Reset()
{
    SetAndLoadValue(resetValue);
}


static Order::E CalculateOrder(const DParam *param)
{
    return param->IsNotOrdered() ? Order::One : param->GetValue().GetOrder();
}


String DParam::ToString(String &units) const
{
    String result(MathDouble::GetIndicatedValue(this));

    units.Set(TypeConversionString::None, GetUnits(CalculateOrder(this)));

    return result;
}


String DParam::ToString(Value val) const
{
    String string(val.ToString(IsSigned()));
    string.Append(" ");
    string.Append(GetUnits(val.GetOrder()));

    return string;
}


String ParameterInteger::ToString(Value val) const
{
    return String(val.ToString(false, Order::One));
}


ParameterInteger::ParameterInteger(ParameterIntegerType::E t, pchar nameRU, pchar nameEN,
    const Value &_min, const Value &_max, pValueInRange _valueInRange, const Value &_value) :
    Param(KindParam::Integer, Param::FuncActive, nameRU, nameEN), tuner(this), type(t),
    min(_min), max(_max), valueInRange(_valueInRange), value(_value), resetValue(_value)
{
}


String ParameterInteger::ToString(String &units) const
{
    units.Free();

    return String("%d", value.Integer());
}


void ParameterInteger::OnPressButtonTune()
{
    PageTuneParameter::SetParameter(this);
    Menu::SetAdditionPage(PageTuneParameter::self);
}



void ParameterChoice::OnPressButtonTune()
{
    NextChoice();
}


void DParam::OnPressButtonTune()
{
    PageTuneParameter::SetParameter(this);
    Menu::SetAdditionPage(PageTuneParameter::self);
}


void ParameterComposite::OnPressButtonTune()
{
    form->OpenCompositeParameter();
}


Value ParameterAmplitude::GetMax() const
{
    // ampl / 2 + fabs(cm) <= 5

    Value offset = form->FindParameter(TypeDParam::Offset)->GetValue();
    offset.SetSign(1);
    offset.Mul(2);

    Value result = DParam::GetMax();
    result.Sub(offset);

    return result;
}


pchar ParameterChoiceType::Name(ParameterChoiceType::E type)
{
    static pchar  const names[ParameterChoiceType::Count] =
    {
        "Polarity",
        "Mode start",
        "Enabled manipulation"
    };

    return names[type];
}


Value ParameterPacketPeriod::CalculateMinValue() const
{
    // Значение периода не может быть меньше (N - 1) * Tи + tи + 10нс

    ParameterPeriod *par_period = (ParameterPeriod *)form->FindParameter(TypeDParam::Period);
    ParameterInteger *par_number = form->FindParameter(ParameterIntegerType::PacketNumber);
    ParameterDuration *par_duration = (ParameterDuration *)form->FindParameter(TypeDParam::Duration);

    if (par_period && par_number && par_duration)
    {
        Value min_value = par_period->GetValue();

        min_value.Mul((uint)(par_number->GetValue().Integer() - 1));

        min_value.Add(par_duration->GetValue());

        min_value.Add(Value("10", Order::Nano));

        return min_value;
    }

    return Value("20", Order::Nano);
}


bool ParameterPacketPeriod::RecalcualateValue()
{
    Value min_value = CalculateMinValue();

    if (GetValue() < min_value)
    {
        SetValue(min_value);

        return true;
    }

    return false;
}

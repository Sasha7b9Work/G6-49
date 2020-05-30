#include "defines.h"
#include "Generator/Generator_p.h"
#include "Generator/MathSupport.h"
#include "Generator/Parameters.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <cstdio>


void Parameter::SetForm(Form *f)
{
    form = f;
}


bool Parameter::IsValue() const
{
    return (kind == Value);
}


bool Parameter::IsComplex() const
{
    return (kind == Complex);
}


bool Parameter::IsChoice() const
{
    return (kind == Choice);
}


pString Parameter::Name() const
{
    return name[LANGUAGE];
}


void Parameter::SetParent(Parameter *p)
{
    parent = p;
}


Parameter *Parameter::GetParent()
{
    return parent;
}

bool Parameter::IsOpened() const
{
    return (parent != nullptr);
}


Form *Parameter::GetForm()
{
    return form;
}


void ParameterComplex::SetForm(Form *f)
{
    Parameter::SetForm(f);

    for (int i = 0; i < numParams; i++)
    {
        params[i]->SetForm(f);
    }
}


ParameterChoice *ParameterComplex::FindParameter(ParameterChoice::E p)
{
    for(int i = 0; i < numParams; i++)
    {
        Parameter *param = params[i];

        if(param->IsChoice())
        {
            ParameterChoice *parameter = static_cast<ParameterChoice *>(param);

            if(parameter->Type() == p)
            {
                return parameter;
            }
        }
    }

    return nullptr;
}


ParameterValue *ParameterComplex::FindParameter(ParameterValueType::E p)
{
    for(int i = 0; i < numParams; i++)
    {
        Parameter *param = params[i];

        if(param->IsValue())
        {
            ParameterValue *parameter = static_cast<ParameterValue *>(param);

            if(parameter->GetType() == p)
            {
                return parameter;
            }
        }
    }

    return nullptr;
}


pString ParameterValue::GetMainUnits() const
{
    static const pString units[ParameterValueType::Count][2] =
    {
        {"Гц", "Hz"},   // Frequency
        {"с",  "s"},    // Period
        {"В",  "V"},    // Amplitude
        {"В",  "V"},    // Offset
        {"с",  "s"},    // Duration
        {"",   ""},     // DutyRatio
        {"",   ""},     // Phase
        {"с",  "s"},    // Delay
        {"с",  "s"},    // DurationRise
        {"с",  "s"},    // DurationFall
        {"с",  "s"},    // DurationStady
        {"",   ""},     // DutyFactor
        {"",   ""},     // ManipulationDuration
        {"",   ""},     // ManipulationPeriod
        {"",   ""},     // PacketPeriod
        {"",   ""},     // PacketNumber
        {"",   ""}      // Exit
    };

    return units[GetType()][LANGUAGE];
}


pString ParameterValue::GetUnits(Order::E order) const
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


bool ParameterValue::SetAndLoadValue(double val)
{
    if(!InRange(val))
    {
        return false;
    }

    value.FromDouble(val);

    PGenerator::SetParameter(this);

    return true;
}


bool ParameterValue::SetAndLoadValue(DoubleValue val)
{
    if (!InRange(val))
    {
        return false;
    }

    value = val;

    PGenerator::SetParameter(this);

    return true;
}


pString ParameterComplex::GetStringValue() const
{
    if(type == Manipulation)
    {
        static pCHAR valuesRU[2] =
        {
            " Откл", " Вкл"
        };

        static pCHAR valuesEN[2] =
        {
            " Off", " On"
        };

        ParameterChoice *enabled = const_cast<ParameterComplex *>(this)->FindParameter(ParameterChoice::ManipulationEnabled);

        return LANG_IS_RU ? valuesRU[enabled->GetChoice()] : valuesEN[enabled->GetChoice()];
    }

    return "";
}


pString ParameterChoice::GetStringValue() const
{
    return names[choice];
}


void ParameterChoice::NextChoice()
{
    Math::CircleIncrease(&choice, 0, NumChoices() - 1);

    Chan::E ch = form->GetWave()->GetChannel();

    if(type == ModeStart)
    {
        PGenerator::LoadStartMode(ch, choice);
    }
    else
    {
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

    PGenerator::SetParameter(this);

    return true;
}


int ParameterChoice::GetChoice() const
{
    return 0;
}


bool ParameterChoice::DrawChoice(int, int) const
{
    return true;
}


ParameterManipulation::ParameterManipulation(Parameter **parameters) : ParameterComplex(ParameterComplex::Manipulation, "Манипуляция", "Manipulation", parameters)
{

}


bool ParameterValue::InRange(double val) const
{
    return (val >= min.ToDouble()) && (val <= max.ToDouble());
}


bool ParameterValue::InRange(DoubleValue val) const
{
    return (val >= min && val <= max);
}


int ParameterChoice::NumChoices() const
{
    return 1;
}


ParameterValue::ParameterValue(ParameterValueType::E t, const char *nameRU, const char *nameEN, const DoubleValue &_min, const DoubleValue &_max, const DoubleValue &_value) :
    Parameter(Parameter::Value, nameRU, nameEN), tuner(this), type(t), min(_min), max(_max), value(_value)
{
}


static Order::E CalculateOrder(const ParameterValue *param)
{
    DoubleValue value = param->GetValue();

    Order::E order = value.GetOrder();

    if (param->IsVoltage() && (value.Abs() == 0))
    {
        order = Order::Milli;
    }

    return order;
}


pString ParameterValue::GetStringValue() const
{
    static char buffer[30];

    std::strcpy(buffer, MathDoubleValue::GetIndicatedValue(this));
    std::strcat(buffer, " ");
    std::strcat(buffer, GetUnits(CalculateOrder(this)));

    return buffer;
}


pString ParameterValue::GetStringValue(DoubleValue) const
{
    return "";
}


pString Order::Suffix(Order::E order)
{
    static const pString suf[Count][2] =
    {
        {"М", "M"},
        {"к",  "k"},
        {"",   ""},
        {"м",  "m"},
        {"мк", "u"},
        {"н",  "n"}
    };

    return suf[order][LANGUAGE];
}


int Order::GetPow10(Order::E order)
{
    static const int pows[Count] = { 6, 3, 0, -3, -6, -9 };

    return pows[order];
}


void ParameterChoice::OnPressButtonTune()
{
    NextChoice();
}


void ParameterValue::OnPressButtonTune()
{
    PageTuneParameter::SetParameter(this);
    Menu::SetAdditionPage(PageTuneParameter::self);
}


void ParameterComplex::OnPressButtonTune()
{
    form->OpenComplexParameter();
}


void Parameter::OnPressButtonTune()
{
    switch (kind)
    {
    case Value:   reinterpret_cast<ParameterValue *>(this)->OnPressButtonTune();   break;
    case Complex: reinterpret_cast<ParameterComplex *>(this)->OnPressButtonTune(); break;
    case Choice:  reinterpret_cast<ParameterChoice *>(this)->OnPressButtonTune();  break;

    case Exit:
    case Page:
        break;
    }
}

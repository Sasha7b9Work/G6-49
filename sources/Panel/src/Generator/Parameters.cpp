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


bool Parameter::IsDouble() const
{
    return (kind == Double);
}


bool Parameter::IsComposite() const
{
    return (kind == Composite);
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


void ParameterComposite::SetForm(Form *f)
{
    Parameter::SetForm(f);

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


ParameterDouble *ParameterComposite::FindParameter(ParameterDoubleType::E p)
{
    for(int i = 0; i < NumParameters(); i++)
    {
        Parameter *param = params[i];

        if(param->IsDouble())
        {
            ParameterDouble *parameter = static_cast<ParameterDouble *>(param);

            if(parameter->GetType() == p)
            {
                return parameter;
            }
        }
    }

    return nullptr;
}


pString ParameterDouble::GetMainUnits() const
{
    static const pString units[ParameterDoubleType::Count][2] =
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


pString ParameterDouble::GetUnits(Order::E order) const
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


bool ParameterDouble::SetAndLoadValue(double val)
{
    if(!InRange(val))
    {
        return false;
    }

    value.FromDouble(val);

    PGenerator::SetParameter(this);

    return true;
}


bool ParameterDouble::SetAndLoadValue(Value val)
{
    if (!InRange(val))
    {
        return false;
    }

    value = val;

    PGenerator::SetParameter(this);

    return true;
}


String ParameterComposite::ToString() const
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

        ParameterChoice *enabled = const_cast<ParameterComposite *>(this)->FindParameter(ParameterChoiceType::ManipulationEnabled);

        return String(LANG_IS_RU ? valuesRU[enabled->GetChoice()] : valuesEN[enabled->GetChoice()]);
    }

    return String("");
}


String ParameterChoice::ToString() const
{
    return String(names[choice]);
}


void ParameterChoice::NextChoice()
{
    Math::CircleIncrease(&choice, 0, NumChoices() - 1);

    Chan::E ch = form->GetWave()->GetChannel();

    if(type == ParameterChoiceType::ModeStart)
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


ParameterManipulation::ParameterManipulation(Parameter **parameters) : ParameterComposite(ParameterComposite::Manipulation, "Манипуляция", "Manipulation", parameters)
{

}


bool ParameterDouble::InRange(double val) const
{
    return (val >= min.ToDouble()) && (val <= max.ToDouble());
}


bool ParameterDouble::InRange(Value val) const
{
    return (val >= min && val <= max);
}


int ParameterChoice::NumChoices() const
{
    return 1;
}


ParameterDouble::ParameterDouble(ParameterDoubleType::E t, const char *nameRU, const char *nameEN, const Value &_min, const Value &_max, const Value &_value) :
    Parameter(Parameter::Double, nameRU, nameEN), tuner(this), type(t), min(_min), max(_max), value(_value)
{
}


static Order::E CalculateOrder(const ParameterDouble *param)
{
    return param->IsVoltage() ? Order::One : param->GetValue().GetOrder();
}


String ParameterDouble::ToString() const
{
    String string(MathValue::GetIndicatedValue(this));
    string.Append(" ");
    string.Append(GetUnits(CalculateOrder(this)));

    return string;
}


String ParameterDouble::ToString(Value val) const
{
    Order::E order = val.GetOrder();

    String string(val.ToString(order, IsSigned()));
    string.Append(" ");
    string.Append(GetUnits(order));

    return string;
}


void ParameterChoice::OnPressButtonTune()
{
    NextChoice();
}


void ParameterDouble::OnPressButtonTune()
{
    PageTuneParameter::SetParameter(this);
    Menu::SetAdditionPage(PageTuneParameter::self);
}


void ParameterComposite::OnPressButtonTune()
{
    form->OpenCompositeParameter();
}

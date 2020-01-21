#include "defines.h"
#include "Generator/Generator_p.h"
#include "Generator/Parameters.h"
#include "Utils/Math.h"
#include <cstdio>


void Parameter::SetForm(Form *f)
{
    form = f;
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


ParameterValue *ParameterComplex::FindParameter(ParameterValue::E p)
{
    for(int i = 0; i < numParams; i++)
    {
        Parameter *param = params[i];

        if(param->IsValue())
        {
            ParameterValue *parameter = static_cast<ParameterValue *>(param);

            if(parameter->Type() == p)
            {
                return parameter;
            }
        }
    }

    return nullptr;
}


pString ParameterValue::GetStringValue(Language::E lang) const
{
    static char buf[100];

    std::snprintf(buf, 99, "%E %s", GetValue().ToFloat(), MainUnits(lang));

    return buf;
}


pString ParameterValue::MainUnits(Language::E lang) const
{
    static const pString units[ParameterValue::Count][Language::Count] =
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
        {"с",  "s"}    // DurationFall
//        {"с",  "s"},    // DurationStady
//        {"",   ""},     // DutyFactor
//        {"",   ""},     // ManipulationDuration
//        {"",   ""},     // ManipulationPeriod
//        {"",   ""},     // PacketPeriod
//        {"",   ""},     // PacketNumber
//        {"",   ""}      // Exit
    };

    return units[Type()][lang];
}


bool ParameterValue::SetAndLoadValue(float val)
{
    if(!InRange(val))
    {
        return false;
    }

    value.FromFloat(val);

    PGenerator::SetParameter(this);

    return true;
}


pString ParameterComplex::GetStringDigits() const
{
    if(type == Manipulation)
    {
        static pCHAR values[2] =
        {
            " Откл", " Вкл"
        };

        ParameterChoice *enabled = const_cast<ParameterComplex *>(this)->FindParameter(ParameterChoice::ManipulationEnabled);

        return values[enabled->GetChoice()];
    }

    return "";
}

pString ParameterChoice::Name() const
{
    static const pString namesParam[ParameterChoice::Count] =
    {
        "Полярность",
        "Запуск",
        "Вид",
        "Манипуляция"
    };

    return namesParam[type];
}


pString ParameterComplex::Name() const
{
    static const pString names[ParameterComplex::Count] =
    {
        "МАНИПУЛЯЦИЯ"
    };

    return names[value];
}


pString ParameterChoice::GetStringDigits() const
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


pString Parameter::NameUnit(char [10]) const
{
    return "";
}


ParameterManipulation::ParameterManipulation(Parameter **parameters) : ParameterComplex(ParameterComplex::Manipulation, parameters)
{

}


pString Parameter::GetStringDigits() const
{
    return "";
}


bool ParameterValue::InRange(float) const
{
    return false;
}


int ParameterChoice::NumChoices() const
{
    return 1;
}

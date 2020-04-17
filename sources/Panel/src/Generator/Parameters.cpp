#include "defines.h"
#include "Generator/Generator_p.h"
#include "Generator/Parameters.h"
#include "Generator/ParametersSupport.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Addition/PageTuneParameter.h"
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


pString Parameter::Name(uint lang) const
{
    return name[lang];
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


pString ParameterValue::MainUnits(uint lang) const
{
    static const pString units[ParameterValue::Count][2] =
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

        if(LANGUAGE)
        {
            return valuesEN[enabled->GetChoice()];
        }

        return valuesRU[enabled->GetChoice()];
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


bool ParameterValue::InRange(float) const
{
    return false;
}


int ParameterChoice::NumChoices() const
{
    return 1;
}


pString ParameterValue::GetStringValue() const
{
    Order order = { Order::Count };

    static char buffer[30];
    std::strcpy(buffer, MathFloatValue::GetStringValue(value, IsSigned(), 5, &order.value, (type == Offset || type == Amplitude) ? 1 : 255));
    std::strcat(buffer, " ");
    std::strcat(buffer, order.Suffix(LANGUAGE));
    std::strcat(buffer, MainUnits(LANGUAGE));

    return buffer;
}


pString Order::Suffix(uint lang) const
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

    return suf[value][lang];
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

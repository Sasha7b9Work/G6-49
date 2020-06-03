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
    return (kind == ParameterKind::Double);
}


bool Parameter::IsInteger() const
{
    return (kind == ParameterKind::Integer);
}


bool Parameter::IsComposite() const
{
    return (kind == ParameterKind::Composite);
}


bool Parameter::IsChoice() const
{
    return (kind == ParameterKind::Choice);
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
            ParameterChoice *choice = static_cast<ParameterChoice *>(param);

            if(choice->GetType() == p)
            {
                return choice;
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
        {"с",  "s"},    // ManipulationDuration
        {"с",  "s"},    // ManipulationPeriod
        {"с",  "s"}     // PacketPeriod
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


bool ParameterDouble::IsVoltage() const
{
    return (type == ParameterDoubleType::Amplitude) || (type == ParameterDoubleType::Offset);
}


bool ParameterDouble::IsTime() const
{
    static const bool isTime[ParameterDoubleType::Count] =
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
        static pCHAR valuesRU[2] =
        {
            "Откл", "Вкл"
        };

        static pCHAR valuesEN[2] =
        {
            "Off", "On"
        };

        ParameterChoice *enabled = const_cast<ParameterComposite *>(this)->FindParameter(ParameterChoiceType::ManipulationEnabled);

        return String(LANG_IS_RU ? valuesRU[enabled->GetChoice()] : valuesEN[enabled->GetChoice()]);
    }

    return String("");
}


String ParameterChoice::ToString(String &units) const
{
    units.Free();

    return String(choices[GetChoice() * 2 + LANGUAGE]);
}


void ParameterChoice::NextChoice()
{
    Math::CircleIncrease(&choice, 0, NumChoices() - 1);

    Chan::E ch = form->GetWave()->GetChannel();

    if(type == ParameterChoiceType::ModeStart)
    {
        PGenerator::LoadStartMode(ch, GetChoice());
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


ParameterManipulation::ParameterManipulation(Parameter **parameters) : ParameterComposite(ParameterCompositeType::Manipulation, "Манипуляция", "Manipulation", parameters)
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
    int count = 0;

    const char *ch = choices[count];

    while (ch != nullptr)
    {
        count++;
        ch = choices[count];
    }

    return count / 2;
}


ParameterDouble::ParameterDouble(ParameterDoubleType::E t, const char *nameRU, const char *nameEN, const Value &_min, const Value &_max, const Value &_value) :
    Parameter(ParameterKind::Double, nameRU, nameEN), tuner(this), type(t), min(_min), max(_max), value(_value)
{
}


static Order::E CalculateOrder(const ParameterDouble *param)
{
    return param->IsVoltage() ? Order::One : param->GetValue().GetOrder();
}


String ParameterDouble::ToString(String &units) const
{
    String result(MathValue::GetIndicatedValue(this));

    units.Set(TypeConversionString::None, GetUnits(CalculateOrder(this)));

    return result;
}


String ParameterDouble::ToString(Value val) const
{
    String string(val.ToString(IsSigned()));
    string.Append(" ");
    string.Append(GetUnits(val.GetOrder()));

    return string;
}


ParameterInteger::ParameterInteger(ParameterIntegerType::E t, const char *nameRU, const char *nameEN, const int _min, const int _max, const int _value) :
    Parameter(ParameterKind::Integer, nameRU, nameEN), tuner(this), type(t), min(_min), max(_max), value(_value)
{
}


String ParameterInteger::ToString(String &units) const
{
    units.Free();

    return String("");
}


void ParameterInteger::OnPressButtonTune()
{

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

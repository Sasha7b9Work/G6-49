#include "defines.h"
#include "Generator/Generator_p.h"
#include "Generator/MathSupport.h"
#include "Generator/Parameters.h"
#include "Generator/Signals.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <cmath>
#include <cstdio>


int ParameterChoice::choiceModeStartFree = 0;


Parameter::Parameter(ParameterKind::E k, const char *nRU, const char *nEN) : viewer(this), form(nullptr), parent(nullptr), kind(k)
{
    names[0] = nRU;
    names[1] = nEN;
}


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
    return names[LANGUAGE];
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
        {"��",   "Hz"},   // Frequency
        {"�",    "s"},    // Period
        {"�",    "V"},    // Amplitude
        {"�",    "V"},    // Offset
        {"�",    "s"},    // Duration
        {"",     ""},     // DutyRatio
        {"\x7b", "\x7b"}, // Phase
        {"�",    "s"},    // Delay
        {"�",    "s"},    // DurationRise
        {"�",    "s"},    // DurationFall
        {"�",    "s"},    // DurationStady
        {"",     ""},     // DutyFactor
        {"�",    "s"},    // ManipulationDuration
        {"�",    "s"},    // ManipulationPeriod
        {"�",    "s"}     // PacketPeriod
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


bool ParameterInteger::SetAndLoadValue(Value val)
{
    if (!InRange(val))
    {
        return false;
    }

    value = val;

    PGenerator::SetParameter(this);

    return true;
}

bool ParameterInteger::SetAndLoadValue(int val)
{
    if (!InRange(Value(val)))
    {
        return false;
    }

    value.FromINT(val);

    PGenerator::SetParameter(this);

    return true;
}


bool ParameterDouble::IsNotOrdered() const
{
    return (type == ParameterDoubleType::Amplitude) || (type == ParameterDoubleType::Offset) || (type == ParameterDoubleType::Phase);
}


bool ParameterDouble::IsPhase() const
{
    return (type == ParameterDoubleType::Phase);
}


bool ParameterDouble::IsTime() const
{
    static const bool isTime[ParameterDoubleType::Count] =
    {
        false,  // �������
        true,   // ������
        false,  // ���������
        false,  // ��������
        true,   // ������������
        false,  // ����������
        false,  // ����� ����
        true,   // ��������
        true,   // ������������ ����������
        true,   // ������������ �����
        true,   // ������������ ������������� ��������
        false,  // ����������� ����������
        true,   // ������������ ��������� � ������ �����������
        true,   // ������ ���������� ��������� � ������ �����������
        true    // ������ ���������� ����� ��������� � �������� ������
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
            "����", "���"
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
    if (type == ParameterChoiceType::ModeStart)
    {
        NextChoiceModeStart();
    }
    else
    {
        Math::CircleIncrease(&choice, 0, NumChoices() - 1);

        Chan::E ch = form->GetWave()->GetChannel();

        PGenerator::TuneChannel(ch);
    }
}


void ParameterChoice::NextChoiceModeStart()
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


//void ParameterChoice::TuneCurrentChoice()
//{
//    if (type == ParameterChoiceType::ModeStart)
//    {
//        if (choiceModeStart > 1 && NumChoices() == 2)
//        {
//            NextChoiceModeStart();
//        }
//    }
//}


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


ParameterManipulation::ParameterManipulation(Parameter **parameters) : ParameterComposite(ParameterCompositeType::Manipulation, "�����", "Manip", parameters)
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


bool ParameterInteger::InRange(Value val) const
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


ParameterDouble::ParameterDouble(ParameterDoubleType::E t, const char *nameRU, const char * const nameEN, const Value &_min, const Value &_max, const Value &_value) :
    Parameter(ParameterKind::Double, nameRU, nameEN), tuner(this), type(t), min(_min), value(_value), resetValue(_value), max(_max)
{
}


void ParameterDouble::Reset()
{
    SetAndLoadValue(resetValue);
}


static Order::E CalculateOrder(const ParameterDouble *param)
{
    return param->IsNotOrdered() ? Order::One : param->GetValue().GetOrder();
}


String ParameterDouble::ToString(String &units) const
{
    String result(MathDouble::GetIndicatedValue(this));

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


String ParameterInteger::ToString(Value val) const
{
    return String(val.ToString(false, Order::One));
}


ParameterInteger::ParameterInteger(ParameterIntegerType::E t, const char *nameRU, const char *nameEN, const Value &_min, const Value &_max, const Value &_value) :
    Parameter(ParameterKind::Integer, nameRU, nameEN), tuner(this), type(t), min(_min), max(_max), value(_value), resetValue(_value)
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


void ParameterDouble::OnPressButtonTune()
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

    Value offset = form->FindParameter(ParameterDoubleType::Offset)->GetValue();
    offset.SetSign(1);
    offset.Mul(2);

    Value result = max;
    result.Sub(offset);

    return result;
}


Value ParameterOffset::GetMax() const
{
    // ���� == 0  | [0 ... 5]
    // ���� <= 1� | [0 ... 2.5], ampl / 2 + fabs(��) <= 2.5
    // ���� > 1�  | [0 ... 5],   ���� / 2 + fabs(��) <= 5

    Value amplitude = form->FindParameter(ParameterDoubleType::Amplitude)->GetValue();

    Value result = max;

    if (amplitude.Abs() == 0)
    {
        return max;
    }
    else if (amplitude.ToDouble() <= 1.0F)
    {
        result.Div(2);
    }

    amplitude.Div(2);

    result.Sub(amplitude);

    return result;
}


Value ParameterOffset::GetMin() const
{
    Value result = GetMax();

    result.SetSign(-1);

    return result;
}


const char *ParameterChoiceType::Name(ParameterChoiceType::E type)
{
    static const char * const names[ParameterChoiceType::Count] =
    {
        "Polarity",
        "Mode start",
        "Enabled manipulation"
    };

    return names[type];
}

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


pString ParameterComplex::GetStringValue() const
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


ParameterManipulation::ParameterManipulation(Parameter **parameters) : ParameterComplex(ParameterComplex::Manipulation, "Манипуляция", parameters)
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
    std::strcat(buffer, order.Suffix(Language::RU));
    std::strcat(buffer, MainUnits(Language::RU));

    return buffer;
}


int MathFloatValue::GetPositionFirstDigit(const FloatValue &_value)
{
    FloatValue value = _value;
    value.SetSign(1);

    int result = 0;

    if(value.Integer() > 0)
    {
        int whole = value.Integer();        // Целая часть числа

        while(whole > 9)
        {
            whole /= 10;
            result++;
        }
    }
    else
    {
        int fract = value.FractNano();

        if(fract == 0)
        {
            return 0;
        }

        do
        {
            result--;
            fract *= 10;
        } while(fract < (1000 * 1000 * 1000));
    }

    return result;
}


int MathFloatValue::GetDigit(const FloatValue &_value, int position)
{
    FloatValue value = _value;
    value.SetSign(1);

    if(position < 0)
    {
        int divider = 100 * 1000 * 1000;       /// На это число будем делить количество наносекунд

        int fract = value.FractNano();

        while(position < -1)
        {
            divider /= 10;
            fract %= divider;
            position++;
        }

        return value.FractNano() / divider;
    }
    else
    {
        int whole = value.Integer();

        while(position > 0)
        {
            whole /= 10;
            position--;
        }

        return (whole % 10);
    }
}


pString MathFloatValue::GetStringValue(const FloatValue &value, bool sign, int numDigits, Order::E *order, int posFirst)
{
    static char buffer[20];

    buffer[numDigits + (sign ? 2 : 1)] = '\0';

    int position = (posFirst == 255) ? GetPositionFirstDigit(value) : posFirst;

    int posComma = PositionComma(position, order);

    if(sign)
    {
        buffer[0] = value.Sign() == 1 ? '+' : '-';
    }

#define POS(i) (sign ? (i + 1) : i)

    for(int i = 0; i <= numDigits; i++)
    {
        if(i == posComma)
        {
            buffer[POS(i)] = '.';
        }
        else
        {
            buffer[POS(i)] = static_cast<char>(GetDigit(value, position)) | 0x30;
            position--;
        }
    }

    return buffer;

}


int MathFloatValue::PositionComma(int posFirstDigit, Order::E *order)
{
    if(posFirstDigit > 5)
    {
        *order = Order::Mega;
    }
    else if(posFirstDigit > 2)
    {
        *order = Order::Kilo;
    }
    else if(posFirstDigit > -1)
    {
        *order = Order::One;
    }
    else if(posFirstDigit > -4)
    {
        *order = Order::Milli;
    }
    else if(posFirstDigit > -7)
    {
        *order = Order::Micro;
    }
    else
    {
        *order = Order::Nano;
    }

    int result = posFirstDigit - 5;

    while(result < 1)
    {
        result += 3;
    }

    return result;
}


pString Order::Suffix(Language::E lang) const
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


bool ParameterEditor::ProcessKey(Parameter *, Key &)
{
    return false;
}


void ParameterChoice::ProcessButtonChange()
{
    NextChoice();
}


void ParameterValue::ProcessButtonChange()
{

}


void ParameterComplex::ProcessButtonChange()
{
    form->OpenCurrentParameter();
}

#include "Command.h"
#include "defines.h"
#include "Log.h"
#include "Settings/Settings.h"
#include "Display/InputWindow.h"
#include "Display/Symbols.h"
#include "Generator.h"
#include "Signals.h"
#include "Utils/Debug.h"
#include "Utils/Math.h"
#include "Menu/Menu.h"
#include "Menu/Pages/AddPageInput.h"
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pString Form::Name(Language lang) const
{
    static const struct StructName
    {
        pString name;
        StructName(pString n) : name(n) { };
    }
    names[Form::Number][2] =
    {
        {"СИНУС",    "SINE"},
        {"ПИЛА+",    "RAMP+"},
        {"ПИЛА-",    "RAMP-"},
        {"МЕАНДР",   "MEANDER"},
        {"ИМПУЛЬС",  "IMPULSE"},
        {"ПАКЕТ",    "PACKET"}
    };

    return names[value][lang].name;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString Register::Name() const
{
    static const struct StrName
    {
        pString name;
        StrName(pString n) : name(n) {};
    }
    names[Register::Number] =
    {
        "Мультиплексор 1",
        "Мультиплексор 2",
        "5697 Смещение 1",
        "5697 Смещение 2",
        "5697 Част уровень",
        "5697 Част гистерезис",
        "RG0 Управление",
        "RG1 Частота",
        "RG2 Умножитель",
        "RG3 Прямоуг А",
        "RG4 Прямоуг B",
        "RG5 Период импульсов А",
        "RG6 Длит. импульсов А",
        "RG7 Период импульсов B",
        "RG8 Длит. импульсов B",
        "RG9 Парам. частотомера",
        "RG10 Смещение",
        "Мультиплексор 3",
        "Частотомер - сопротивление",
        "Частотомер - связь",
        "Частотомер - фильтр"
    };

    return names[value].name;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool ParameterValue::IsComplexParameter() const
{
    return value == Manipulation;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString ParameterValue::Name() const
{
    static const struct StructName
    {
        pString name;
        StructName(pString n) : name(n) { };
    }
    nameParameter[ParameterValue::Number][2] =
    {
        {"Частота",        "Frequency"},
        {"Период",         "Period"},
        {"Размах",         "Amplitude"},
        {"Смещение",       "Offset"},
        {"Длительность",   "Duration"},
        {"Скважность",     "Duty ratio"},
        {"Фаза",           "Phapse"},
        {"Задержка",       "Delay"},
        {"Вр нарастания",  "Build-up time"},
        {"Вр спада",       "Release time"},
        {"Вр пика",        "Peak time"},
        {"Коэфф заполн",   "Duty factor"},
        {"Манипуляция",    "Manipulation"},
        {"Длительность",   "Duration"},
        {"Период",         "Period"},
        {"Период пакета",  "Packet period"},
        {"Кол-во имп",     "Num of puls"},
        {"     Выход ( ESC )", "     Exit ( ESC )"}
    };

    return nameParameter[value][LANG].name;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Form *Wave::GetCurrentForm()
{
    return &forms[currentForm];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Wave::SetNextForm()
{
    currentForm++;
    if(currentForm >= NumberOfForms())
    {
        currentForm = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Wave::NumberOfForms() const
{
    return numForms;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Wave::Wave(Chan ch, Form *f, int num) : channel(ch), forms(f), numForms(num)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Form::Form(E v, ParameterBase *parameters[], Wave *w) : value(v), wave(w), params(parameters), currentParam(0)
{
    numParams = 0;

    if(params)
    {
        while(params[numParams])
        {
            numParams++;
        }
    }

    for(int i = 0; i < numParams; i++)
    {
        params[i]->form = this;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
ParameterBase *Form::CurrentParameter()
{
    return params[currentParam];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Form::NumParameters() const 
{
    return numParams;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
ParameterBase *Form::GetParameter(int i)
{
    if(i < numParams)
    {
        return params[i];
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Form *Wave::GetForm(int i)
{
    return &forms[i];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Form::SetNextParameter()
{
    currentParam++;
    if(currentParam >= NumParameters())
    {
        currentParam = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Form::TuneGenerator(Chan ch)
{
    Generator::SetFormWave(GetWave());

    if(value == Form::Sine)
    {
        if(set.sineManipulation[ch])
        {
            int current = currentParam;
            ParameterBase **param = params;
            int numPar = numParams;

            currentParam = oldCurrentParams;
            params = oldParams;
            numParams = oldNumParams;

            if(PARAM_CURRENT_IS_MANIPULATION)
            {
                SendParameterToGenerator(ParameterValue::Frequency);
                SendParameterToGenerator(ParameterValue::Amplitude);
                SendParameterToGenerator(ParameterValue::Offset);
                SendParameterToGenerator(ParameterValue::Manipulation);
            }

            currentParam = current;
            params = param;
            numParams = numPar;

            SendParameterToGenerator(ParameterValue::ManipulationDuration);
            SendParameterToGenerator(ParameterValue::ManipulationPeriod);
        }
        else
        {
            SendParameterToGenerator(ParameterValue::Manipulation);
            SendParameterToGenerator(ParameterValue::Frequency);
            SendParameterToGenerator(ParameterValue::Amplitude);
            SendParameterToGenerator(ParameterValue::Offset);
        }
        if(!ch.IsA())
        {
            SendParameterToGenerator(ParameterValue::Phase);
        }
    }
    else
    {
        SendParameterToGenerator(ParameterValue::Frequency);
        SendParameterToGenerator(ParameterValue::Amplitude);
        SendParameterToGenerator(ParameterValue::Offset);
        SendParameterToGenerator(ParameterValue::Period);
        SendParameterToGenerator(ParameterValue::Duration);
        SendParameterToGenerator(ParameterValue::PacketNumber);
        SendParameterToGenerator(ParameterValue::PacketPeriod);
        SendParameterToGenerator(ParameterChoice::Polarity);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
ParameterBase *Form::FindParameter(ParameterValue::E p)
{
    for(int i = 0; i < numParams; i++)
    {
        ParameterBase *param = params[i];

        if(param->IsValue() && ((ParameterValue *)param)->value == p)
        {
            return param;
        }
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
ParameterBase *Form::FindParameter(ParameterChoice::E p)
{
    for(int i = 0; i < numParams; i++)
    {
        ParameterBase *param = params[i];
        if(param->IsChoice() && ((ParameterChoice *)param)->value == p)
        {
            return param;
        }
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Form::SendParameterToGenerator(ParameterValue::E p)
{
    ParameterBase *param = FindParameter(p);
    if (param)
    {
        Generator::SetParameter((ParameterValue *)param);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Form::SendParameterToGenerator(ParameterChoice::E p)
{
    ParameterBase *param = FindParameter(p);
    if(param)
    {
        Generator::SetParameter((ParameterChoice *)param);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Form::OpenCurrentParameter()
{
    if(!PARAM_CURRENT_IS_COMPLEX)
    {
        return;
    }

    if(PARAM_CURRENT_IS_MANIPULATION)
    {
        /// Если у этого параметра есть родитель, значит, этот параметр управляет включением/отключением манипуляции
        if(PARAM_CURRENT->GetParent())
        {
            set.sineManipulation[CURRENT_CHANNEL] = !set.sineManipulation[CURRENT_CHANNEL];
            Generator::TuneChannel(GetWave()->GetChannel());
        }
        else
       {
            oldParams = params;
            oldNumParams = numParams;
            oldCurrentParams = currentParam;

            ParameterBase *parent = CurrentParameter();

            numParams = ((ParameterValue *)parent)->numParams;
            params = ((ParameterValue *)parent)->params;
            currentParam = 0;

            for (int i = 0; i < numParams; i++)
            {
                params[i]->form = this;
                params[i]->parent = parent;
            }
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Form::CloseOpenedParameter()
{
    if (ParameterIsOpened())
    {
        params = oldParams;
        numParams = oldNumParams;
        currentParam = oldCurrentParams;
        return true;
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Form::ParameterIsOpened() const
{
    return params[0]->parent != 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float ParameterValue::GetValue() const
{
    if(Is(Manipulation))
    {
        ParameterValue *pointer = (ParameterValue *)this;
        return set.sineManipulation[pointer->GetForm()->GetWave()->GetChannel()] ? 1.0f : 0.0f;
    }
    StructValue input((ParameterValue *)this);
    return input.Value();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString ParameterValue::GetStringValue() const
{
    if(Is(Manipulation))
    {
        static const char *values[2][2] =
        {
            {" Откл", " Вкл"},
            {" Off",  " On"}
        };

        ParameterValue *pointer = (ParameterValue *)this;
        return values[LANG][set.sineManipulation[pointer->GetForm()->GetWave()->GetChannel()] ? 1 : 0];
    }
    StructValue input((ParameterValue *)this);
    return input.StringValue();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString ParameterValue::NameUnit(char buf[10]) const
{
    static const struct StructName
    {
        pString name;
        StructName(pString n) : name(n) {};
    }
    names[ParameterValue::Number][2] =
    {
        {"Гц",  "Hz"},
        {"с",   "s"},
        {"В",   "V"},
        {"В",   "V"},
        {"с",   "s"},
        {"",    ""},
        {"\x7b","\x7b"},
        {"с",   "s"},
        {"",    ""},
        {"",    ""},
        {"",    ""},
        {"",    ""},
        {"",    ""},
        {"c",   "s"},
        {"c",   "s"},
        {"c",   "s"},
        {"",    ""},
        {"",    ""}
    };

    sprintf(buf, "%s%s", order.Name(), names[value][LANG].name);
    return buf;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString ParameterBase::NameUnit(char buffer[10]) const
{
    if(IsValue())
    {
        return ((ParameterValue *)this)->NameUnit(buffer);
    }
    return "";
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool ParameterValue::IsOpened()
{
    return IsComplexParameter() && GetForm()->ParameterIsOpened();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString ParameterChoice::Name() const
{
    static const struct StructName
    {
        pString name;
        StructName(pString n) : name(n) {};
    }
    namesParam[ParameterChoice::Number][2] =
    {
        {"Полярность", "Polarity"},
        {"Запуск", "Mode start"}
    };

    return namesParam[value][LANG].name;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString ParameterBase::Name() const
{
    if(IsValue())
    {
        return ((ParameterValue *)this)->Name();
    }
    else if(IsChoice())
    {
        return ((ParameterChoice *)this)->Name();
    }

    return "";
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString ParameterBase::GetStringValue() const
{
    if(IsValue())
    {
        return ((ParameterValue *)this)->GetStringValue();
    }
    else if(IsChoice())
    {
        return ((ParameterChoice *)this)->GetStringValue();
    }
    return "";
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString ParameterChoice::GetStringValue() const
{
    return names[choice][LANG];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Form::ChangeParameter()
{
    ParameterBase *param = CurrentParameter();

    if(param->IsChoice())
    {
        ((ParameterChoice *)param)->NextChoice();
    }
    else if(param->IsValue())
    {
        if(((ParameterValue *)param)->IsInputValue())
        {
            InputWindow::Init();
            Menu::SetAdditionPage((Page *)AddPageInput::pointer);
        }
    }
    else if (PARAM_CURRENT_IS_EXIT)
    {
        CloseOpenedParameter();
    }
    else if (PARAM_CURRENT_IS_COMPLEX)
    {
        OpenCurrentParameter();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ParameterChoice::NextChoice()
{
    CircleIncrease(&choice, 0, num - 1);

    Chan ch = form->GetWave()->GetChannel();

    if(value == ModeStart)
    {
        Generator::LoadStartMode(ch, choice);
    }
    else
    {
        Generator::TuneChannel(ch);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool ParameterChoice::DrawChoice(int x, int y)
{
    if(func[choice])
    {
        func[choice](x, y);
    }

    return func[choice] != 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Wave::StartModeIsSingle()
{
    ParameterChoice* param = (ParameterChoice *)GetCurrentForm()->FindParameter(ParameterChoice::ModeStart);

    if(param)
    {
        return param->GetChoice() == 1;
    }

    return false;
}

#include "Command.h"
#include "defines.h"
#include "Log.h"
#include "Settings/Settings.h"
#include "Display/InputWindow.h"
#include "Generator.h"
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct StructName
{
    pString nameRU;
    pString nameEN;
    StructName(pString nRU, pString nEN) : nameRU(nRU), nameEN(nEN) {};
}
nameParameter[Parameter::Number] =
{
    StructName("×ÀÑÒÎÒÀ",        "FREQUENCY"),
    StructName("ÏÅÐÈÎÄ",         "PERIOD"),
    StructName("ÐÀÇÌÀÕ",         "AMPLITUDE"),
    StructName("ÑÌÅÙÅÍÈÅ",       "OFFSET"),
    StructName("ÄËÈÒ.",          "DURATION"),
    StructName("ÑÊÂÀÆÍÎÑÒÜ",     "DUTY RATIO"),
    StructName("ÔÀÇÀ",           "PHASE"),
    StructName("ÇÀÄÅÐÆÊÀ",       "DELAY"),
    StructName("ÊÎÝÔÔ. ÌÎÄ.",    "MOD. INDEX"),
    StructName("ÏÎËßÐÍÎÑÒÜ",     "POLARITY"),
    StructName("ÂÐ. ÍÀÐÀÑÒÀÍÈß", "BUILD-UP TIME"),
    StructName("ÂÐ. ÑÏÀÄÀ",      "RELEASING TIME"),
    StructName("ÂÐ. ÏÈÊÀ",       "PEAK TIME"),
    StructName("ÊÎÝÔÔ. ÇÀÏÎËÍ.", "DUTY FACTOR"),
    StructName("Ìîäóëÿöèÿ",      "Modulation")
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pString Form::Name(Language lang) const
{
    struct StrName
    {
        pString nameRu;
        pString nameEn;
        StrName(pString nRu, pString nEn) : nameRu(nRu), nameEn(nEn) { };
        pString Name(Language lang) { return lang == Language::RU ? nameRu : nameEn; }
    };

    static StrName names[Form::Number] =
    {
        StrName("ÑÈÍÓÑ",    "SINE"),
        StrName("ÏÈËÀ+",    "RAMP+"),
        StrName("ÏÈËÀ-",    "RAMP-"),
        StrName("ÌÅÀÍÄÐ",   "MEANDER"),
        StrName("ÈÌÏÓËÜÑ",  "IMPULSE")
    };

    return names[value].Name(lang);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString Register::Name() const
{
    struct StrName
    {
        pString name;
        StrName(pString n) : name(n) {};
    };

    static const StrName names[Register::Number] =
    {
        "Ìóëüòèïëåêñîð 1",
        "Ìóëüòèïëåêñîð 2",
        "5697 Ñìåùåíèå 1",
        "5697 Ñìåùåíèå 2",
        "5697 ×àñò óðîâåíü",
        "5697 ×àñò ãèñòåðåçèñ",
        "RG0 Óïðàâëåíèå",
        "RG1 ×àñòîòà",
        "RG2 Óìíîæèòåëü",
        "RG3 Ïðÿìîóã À",
        "RG4 Ïðÿìîóã B",
        "RG5 Ïåðèîä èìïóëüñîâ À",
        "RG6 Äëèò. èìïóëüñîâ À",
        "RG7 Ïåðèîä èìïóëüñîâ B",
        "RG8 Äëèò. èìïóëüñîâ B",
        "RG9 Ïàðàì. ÷àñòîòîìåðà",
        "RG10 Ñìåùåíèå",
        "Ìóëüòèïëåêñîð 3",
        "×àñòîòîìåð - ñîïðîòèâëåíèå",
        "×àñòîòîìåð - ñâÿçü",
        "×àñòîòîìåð - ôèëüòð"
    };

    return names[value].name;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Parameter::IsComplexParameter() const
{
    return MinValue() == -1.0f && MaxValue() == -1.0f;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString Parameter::Name() const
{
    return LANG_RU ? (char *)nameParameter[value].nameRU : (char *)nameParameter[value].nameEN;
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
    for(int i = 0; i < Form::Number; i++)
    {
        if(forms[i].value == Form::Number)
        {
            return i;
        }
    }

    return Form::Number;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Wave::Wave(Chan ch, Form form[Form::Number]) : channel(ch)
{
    currentForm = 0;

    for(int i = 0; i < Form::Number; i++)
    {
        forms[i] = form[i];
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Form::Form(E v, Parameter param[Parameter::Number]) : value(v), currentParam(0)
{
    for(int i = 0; i < Parameter::Number; i++)
    {
        params[i] = param[i];
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Parameter *Form::CurrentParameter()
{
    return &params[currentParam];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Form::NumParameters() const 
{
    for(int i = 0; i < Parameter::Number; i++)
    {
        if(params[i].Is(Parameter::Number))
        {
            return i;
        }
    }

    return Parameter::Number - 1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Parameter *Form::GetParameter(int i)
{
    return &params[i];
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
    Generator::SetFormWave(ch, *this);

    SendParameterToGenerator(ch, Parameter::Frequency);
    SendParameterToGenerator(ch, Parameter::Amplitude);
    SendParameterToGenerator(ch, Parameter::Offset);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Parameter *Form::FindParameter(Parameter::E p)
{
    for(int i = 0; i < Parameter::Number; i++)
    {
        if(params[i].value == p)
        {
            return &params[i];
        }
    }
    return 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Form::SendParameterToGenerator(Chan ch, Parameter::E p)
{
    Parameter *param = FindParameter(p);
    if (param)
    {
        InputWindow::Struct input(this, param);
        Generator::SetParameter(ch, p, input.Value());
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Form::OpenCurrentParameter()
{
    if(!CurrentParameter()->IsComplexParameter())
    {
        return;
    }


}

#include "Command.h"
#include "defines.h"
#include "Log.h"
#include "Settings/Settings.h"
#include "Display/InputWindow.h"
#include "Generator.h"
#include <stdio.h>


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
    return numParams != 0;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString Parameter::Name() const
{
    static const struct StructName
    {
        pString nameRU;
        pString nameEN;
        StructName(pString nRU, pString nEN) : nameRU(nRU), nameEN(nEN) { };
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

    return LANG_RU ? nameParameter[value].nameRU : nameParameter[value].nameEN;
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
Form::Form(E v, Parameter *parameters, int num) : value(v), params(parameters), numParams(num), currentParam(0)
{
    for(int i = 0; i < numParams; i++)
    {
        params[i].form = this;
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
    return numParams;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Parameter *Form::GetParameter(int i)
{
    if(i < numParams)
    {
        return &params[i];
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
    Generator::SetFormWave(ch, *this);

    SendParameterToGenerator(ch, Parameter::Frequency);
    SendParameterToGenerator(ch, Parameter::Amplitude);
    SendParameterToGenerator(ch, Parameter::Offset);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Parameter *Form::FindParameter(Parameter::E p)
{
    for(int i = 0; i < numParams; i++)
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
        Generator::SetParameter(ch, p, param->GetValue());
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Form::OpenCurrentParameter()
{
    if(!CurrentParameter()->IsComplexParameter())
    {
        return;
    }

    oldParams = params;
    oldNumParams = numParams;
    oldCurrentParams = currentParam;

    Parameter *parent = CurrentParameter();

    numParams = CurrentParameter()->numParams;
    params = CurrentParameter()->params;
    currentParam = 0;

    for(int i = 0; i < numParams; i++)
    {
        params[i].form = this;
        params[i].parent = parent;
    }

    if(parent->value == Parameter::ModulationRampSine)
    {
        //Generator::Ty
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Form::CloseOpenedParameter()
{
    if (params[0].parent)
    {
        params = oldParams;
        numParams = oldNumParams;
        currentParam = oldCurrentParams;
        return true;
    }

    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float Parameter::GetValue() const
{
    StructValue input((Parameter *)this);
    return input.Value();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char *Parameter::GetStringValue() const
{
    StructValue input((Parameter *)this);
    return input.StringValue();
}

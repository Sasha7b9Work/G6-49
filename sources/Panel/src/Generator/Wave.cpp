#include "Command.h"
#include "defines.h"
#include "Log.h"
#include "Settings/Settings.h"
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static struct StructName
{
    pString nameRU;
    pString nameEN;
    StructName(pString nRU, pString nEN) : nameRU(nRU), nameEN(nEN) {};
}
nameParameter[Form::Parameter::Number] =
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
static const struct StructMinMax
{
    float min;
    float max;
    StructMinMax(float _min, float _max) : min(_min), max(_max) {};
}
minMax[Form::Parameter::Number] =
{
    StructMinMax(1.0f,    50e6f),       // Frequency
    StructMinMax(2e-08f,  1.0f),        // Period
    StructMinMax(0.000f,  65535.0f),    // Amplitude
    StructMinMax(0.0f,    4095.0f),     // Offset
    StructMinMax(2e-08f,  10.0f),       // Duration
    StructMinMax(0.001f,  1.0f),        // DutyRatio
    StructMinMax(-180.0f, 180.0f),      // Phase
    StructMinMax(2e-08f,  10e08f),      // Delay
    StructMinMax(1e-3f,   10e6f),       // DepthModulation
    StructMinMax(-1,      1),           // Polarity
    StructMinMax(1e-6f,   1e6f),        // DurationRise
    StructMinMax(1e-6f,   1e6f),        // DurationFall
    StructMinMax(1e-6f,   1e6f),        // DurationStady
    StructMinMax(1e-6f,   1e6f),        // DutyFactor
    StructMinMax(-1.0f,   -1.0f)        // Modulation [-1.0f;1.0f] îçíà÷àåò, ÷òî ýòîò ïàðàìåòð èìååò íåñêîëüêî ïàðàìåòðîâ - âûáîð åãî îòêðûâàåò
                                        // ñòðàíèöó óñòàíîâêè ïàðàìåòðîâ
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Form::Parameter::IsPage() const
{
    return MinValue() == -1.0f && MaxValue() == -1.0f;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float Form::Parameter::MinValue() const
{
    return minMax[value].min;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float Form::Parameter::MaxValue() const
{
    return minMax[value].max;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString Form::Parameter::Name() const
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
Form::Form(E v, Parameter param[Form::Parameter::Number]) : value(v), currentParam(0)
{
    for(int i = 0; i < Form::Parameter::Number; i++)
    {
        params[i] = param[i];
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Form::Parameter *Form::CurrentParameter()
{
    return &params[currentParam];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Form::NumParameters() const 
{
    for(int i = 0; i < Form::Number; i++)
    {
        if(params[i] == Form::Number)
        {
            return i;
        }
    }

    return Form::Number - 1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Form::Parameter *Form::GetParameter(int i)
{
    return &params[i];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Form *Wave::GetForm(int i)
{
    return &forms[i];
}

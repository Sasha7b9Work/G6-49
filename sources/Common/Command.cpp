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
nameParameter[Wave::Parameter::Number] =
{
    StructName("ЧАСТОТА",        "FREQUENCY"),
    StructName("ПЕРИОД",         "PERIOD"),
    StructName("РАЗМАХ",         "AMPLITUDE"),
    StructName("СМЕЩЕНИЕ",       "OFFSET"),
    StructName("ДЛИТ.",          "DURATION"),
    StructName("СКВАЖНОСТЬ",     "DUTY RATIO"),
    StructName("ФАЗА",           "PHASE"),
    StructName("ЗАДЕРЖКА",       "DELAY"),
    StructName("КОЭФФ. МОД.",    "MOD. INDEX"),
    StructName("ПОЛЯРНОСТЬ",     "POLARITY"),
    StructName("ВР. НАРАСТАНИЯ", "BUILD-UP TIME"),
    StructName("ВР. СПАДА",      "RELEASING TIME"),
    StructName("ВР. ПИКА",       "PEAK TIME"),
    StructName("КОЭФФ. ЗАПОЛН.", "DUTY FACTOR"),
    StructName("Модуляция",      "Modulation")
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pString Wave::Form::Name(Language lang) const
{
    struct StrName
    {
        pString nameRu;
        pString nameEn;
        StrName(pString nRu, pString nEn) : nameRu(nRu), nameEn(nEn) { };
        pString Name(Language lang) { return lang == Language::RU ? nameRu : nameEn; }
    };

    static StrName names[Wave::Form::Number] =
    {
        StrName("СИНУС",    "SINE"),
        StrName("ПИЛА+",    "RAMP+"),
        StrName("ПИЛА-",    "RAMP-"),
        StrName("МЕАНДР",   "MEANDER"),
        StrName("ИМПУЛЬС",  "IMPULSE")
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
static const struct StructMinMax
{
    float min;
    float max;
    StructMinMax(float _min, float _max) : min(_min), max(_max) {};
}
minMax[Wave::Parameter::Number] =
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
    StructMinMax(-1.0f,   -1.0f)        // Modulation [-1.0f;1.0f] означает, что этот параметр имеет несколько параметров - выбор его открывает
                                        // страницу установки параметров
};

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Wave::Parameter::IsPage() const
{
    return MinValue() == -1.0f && MaxValue() == -1.0f;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float Wave::Parameter::MinValue() const
{
    return minMax[value].min;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float Wave::Parameter::MaxValue() const
{
    return minMax[value].max;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString Wave::Parameter::Name() const
{
    return LANG_RU ? (char *)nameParameter[value].nameRU : (char *)nameParameter[value].nameEN;
}

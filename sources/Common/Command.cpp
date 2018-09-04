#include "Command.h"
#include "defines.h"
#include "Log.h"
#include "Settings/Settings.h"
#include <stdio.h>


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
        StrName("СИНУС",        "SINE"),
        StrName("ПИЛА+",        "RAMP+"),
        StrName("ПИЛА-",        "RAMP-"),
        StrName("МЕАНДР",       "MEANDER")
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

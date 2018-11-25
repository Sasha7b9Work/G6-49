#include "CommonTypes.h"
#include "defines.h"
#include "Wave.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pString Form::Name(Language lang) const
{
    DEF_STRUCT(StructName, pString) names[Form::Number][2] =
    {
        {"Синус",        "Sine"},
        {"Пила+",        "Ramp+"},
        {"Пила-",        "Ramp-"},
        {"Треугольник",  "Triangle"},
        {"Меандр",       "Meander"},
        {"Импульс",      "Impulse"},
        {"Пакет",        "Message"},
        {"Произвольный", "Hand"}
    };

    return names[value][lang].val;
}



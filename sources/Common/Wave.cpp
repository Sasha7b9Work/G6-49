#include "CommonTypes.h"
#include "defines.h"
#include "Wave.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pString Form::Name(Language lang) const
{
    static const struct StructName
    {
        pString name;
        StructName(pString n) : name(n)
        {
        };
    }
    names[Form::Number][2] =
    {
        {"Синус",    "Sine"},
        {"Пила+",    "Ramp+"},
        {"Пила-",    "Ramp-"},
        {"Меандр",   "Meander"},
        {"Импульс",  "Impulse"},
        {"Пакет",    "Packet"}
    };

    return names[value][lang].name;
}

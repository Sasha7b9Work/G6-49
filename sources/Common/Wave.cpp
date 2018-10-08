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
        {"—»Õ”—",    "SINE"},
        {"œ»À¿+",    "RAMP+"},
        {"œ»À¿-",    "RAMP-"},
        {"Ã≈¿Õƒ–",   "MEANDER"},
        {"»Ãœ”À‹—",  "IMPULSE"},
        {"œ¿ ≈“",    "PACKET"}
    };

    return names[value][lang].name;
}

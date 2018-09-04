#include "Command.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef PANEL
pString Wave::Name(uint num, Language lang)
{
    static const pString names[Wave::Form::Number][2] =
    {
        {"Синус", "Sine"},
        {"Косинус", "Cosine"}
    };

    return names[num][lang];
}
#endif

#include "Wave.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pString Wave::Name(uint num, Language lang)
{
    static const pString names[WaveForm::Number][2] =
    {
        {"Синус", "Sine"},
    {"Косинус", "Cosine"}
    };

    return names[num][lang];
}

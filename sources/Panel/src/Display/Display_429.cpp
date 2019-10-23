#include "defines.h"
#include "Display/Display.h"
#include "Hardware/LTDC.h"
#include "Display/Text.h"



void Display::Init()
{
    LTDC_::Init((uint)frontBuffer, (uint)backBuffer);

    Text::SetUpperCase(true);

    Font::SetType(Font::Type::_8);
}

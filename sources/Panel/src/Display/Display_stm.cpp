#include "defines.h"
#include "Display/Display.h"
#include "Display/Font/Font.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Text.h"



void Display::Init()
{
    HAL_LTDC::Init((uint)frontBuffer, (uint)backBuffer);

    Font::ForceUpperCase(true);

    Font::Set(TypeFont::_8);
}

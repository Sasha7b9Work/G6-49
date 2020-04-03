#include "defines.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"



void Display::Init()
{
    HAL_LTDC::Init(reinterpret_cast<uint>(frontBuffer), reinterpret_cast<uint>(backBuffer));
}

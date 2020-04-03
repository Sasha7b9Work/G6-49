#include "defines.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Hardware/CPU.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/Keyboard.h"
#include <cstdlib>


uint8 Display::frontBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];

uint8 Display::backBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];



uint8 *Display::GetBuffer()
{
    return backBuffer;
}


void Display::Update()
{
    Painter::BeginScene(Color::BACK);
    Painter::EndScene();
}


void Display::Init()
{
    HAL_LTDC::Init(reinterpret_cast<uint>(frontBuffer), reinterpret_cast<uint>(backBuffer));
}

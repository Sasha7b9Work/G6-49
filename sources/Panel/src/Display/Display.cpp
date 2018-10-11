#include "Console.h"
#include "FrequencyMeter/FrequencyMeter.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Hardware/CPU.h"
#include "Hardware/LTDC.h"
#include "InputWindow.h"
#include "Menu/Hint.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageDebug.h"
#include "Utils/Statistics.h"
#include "Utils/Debug.h"
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef OPEN
uint8       *Display::frontBuffer = (uint8 *)SDRAM_DEVICE_ADDR;
uint8       *Display::backBuffer = (uint8 *)(SDRAM_DEVICE_ADDR + BUFFER_HEIGHT * BUFFER_WIDTH);
#else
uint8   Display::frontBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];
uint8   Display::backBuffer[BUFFER_WIDTH * BUFFER_HEIGHT];
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Init()
{
    LTDC_::Init((uint)frontBuffer, (uint)backBuffer);
    
    Text::SetUpperCase(true);
    
    Font::SetType(Font::Type::_8);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Display::GetBuffer()
{
    return backBuffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Update()
{
    Statistics::BeginFrame();
    Painter::BeginScene(Color::BACK);
    Wave::Graphics::Draw(Chan::A);
    Wave::Graphics::Draw(Chan::B);
    Menu::Draw();
    PageDebug::PageRegisters::Draw();
    Painter::DrawRectangle(0, 0, 318, 238, Color::FILL);
    FrequencyMeter::Draw();
    Hint::Draw();
    Statistics::Show();
    Console::Draw();
    CPU::Keyboard::Draw();
    Painter::EndScene();
    Statistics::EndFrame();
}

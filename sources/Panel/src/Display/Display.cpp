#include "Console.h"
#include "FrequencyMeter/FrequencyMeter.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Hardware/CPU.h"
#include "Hardware/LTDC.h"
#include "InputWindow.h"
#include "Menu/Hint.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageService.h"
#include "Utils/Statistics.h"
#include "Utils/Debug.h"
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef OPEN
uint8       *Display::frontBuffer = (uint8 *)SDRAM_DEVICE_ADDR;
uint8       *Display::backBuffer = (uint8 *)(SDRAM_DEVICE_ADDR + BUFFER_HEIGHT * BUFFER_WIDTH);
#else
uint8       *Display::frontBuffer = (uint8 *)malloc(BUFFER_WIDTH * BUFFER_HEIGHT);
uint8       *Display::backBuffer = (uint8 *)malloc(BUFFER_WIDTH * BUFFER_HEIGHT);
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
    DEBUG_POINT;
    Statistics::BeginFrame();
    DEBUG_POINT;
    Painter::BeginScene(Color::BACK);
    DEBUG_POINT;
    Wave::Graphics::Draw(Chan::A);
    DEBUG_POINT;
    //Wave::Graphics::Draw(Chan::B);
    DEBUG_POINT;
    Menu::Draw();
    DEBUG_POINT;
    PageService::PageRegisters::Draw();
    Painter::DrawRectangle(0, 0, 318, 238, Color::FILL);
    FrequencyMeter::Draw();
    Hint::Draw();
    Statistics::Show();
    Console::Draw();
    CPU::Keyboard::Draw();
    Painter::EndScene();
    Statistics::EndFrame();
    DEBUG_POINT;
}

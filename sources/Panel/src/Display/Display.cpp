#include "defines.h"
#include "globals.h"
#include "Console.h"
#include "Wave.h"
#include "Display.h"
#include "DisplayTypes.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Display/Font/Font.h"
#include "Hardware/CPU.h"
#include "Hardware/LTDC.h"
#include "Hardware/Timer.h"
#include "InputWindow.h"
#include "InputWindowStruct.h"
#include "Menu/Hint.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Settings/SettingsDisplay.h"
#include "Settings/SettingsSignals.h"
#include "Settings/SettingsTypes.h"
#include "Utils/StringUtils.h"
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef STM32F429xx
uint8       *Display::frontBuffer = (uint8 *)malloc(BUFFER_WIDTH * BUFFER_HEIGHT);
uint8       *Display::backBuffer = (uint8 *)malloc(BUFFER_WIDTH * BUFFER_HEIGHT);
#endif
    
#ifdef STM32F746xx
uint8       *Display::frontBuffer = (uint8 *)SDRAM_DEVICE_ADDR;
uint8       *Display::backBuffer = (uint8 *)(SDRAM_DEVICE_ADDR + BUFFER_HEIGHT * BUFFER_WIDTH);
#endif

int Display::timeFrame = 0;
int Display::timeAllFrames = 0;
uint Display::timeStartFrames = 0;
uint Display::timeAccumFrames = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Init()
{
    LTDC_::Init((uint)frontBuffer, (uint)backBuffer);
    
    Text::SetUpperCase(true);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *Display::GetBuffer()
{
    return backBuffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Update()
{
    uint timeStart = TIME_MS;

    Painter::BeginScene(Color::BACK);

    Painter::SetColor(Color::FILL);

    Wave::DrawSignal(A);

    Wave::DrawSignal(B);

    Menu::Draw();

    if (ADDITION_PAGE_IS_INPUT)
    {
        InputWindow::Draw();
    }

    Painter::DrawRectangle(0, 0, 318, 238, Color::FILL);

    DrawFrequencyCounter();

    Hint::Draw();

    ShowStatistics();

    Console::Draw();

    CPU::Keyboard::Draw();

    Painter::EndScene();

    timeFrame = (int)(TIME_MS - timeStart);

    timeAccumFrames += timeFrame;

    if(TIME_MS >= timeStartFrames + 1000)
    {
        timeAllFrames = (int)timeAccumFrames;
        timeStartFrames = TIME_MS;
        timeAccumFrames = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawFrequencyCounter()
{
    if(!FREQ_COUNTER_ENABLED)
    {
        return;
    }

    Text::DrawText(8, SCREEN_HEIGHT - 16, "F", Color::FILL);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::ShowStatistics()
{
    if(SHOW_STATISTICS)
    {
        Painter::DrawFilledRectangle(0, SCREEN_HEIGHT - 12, 60, 10, Color::BACK, Color::FILL);
        char buffer[100] = {0};
        strcpy(buffer, Int2String((int)(CPU::GetFPS() + 0.5f), false, 3));
        strcat(buffer, "/");
        strcat(buffer, Int2String(timeFrame, false, 3));
        strcat(buffer, "/");
        strcat(buffer, Int2String(timeAllFrames, false, 3));
        Text::DrawText(2, SCREEN_HEIGHT - 11, buffer, Color::FILL);
    }
}

#include "defines.h"
#include "globals.h"
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
char        Display::bufferConsole[STRING_IN_CONSOLE][SYMBOLS_IN_STRING] = {};
int         Display::stringsInConsole = 0;
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

    DrawConsole();

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
void Display::AddStringToConsole(char *string)
{
    // Находим номер первой пустой строки
    int numEmptyString = 0;

    for (; numEmptyString < STRING_IN_CONSOLE; numEmptyString++)
    {
        if (!bufferConsole[numEmptyString][0])
        {
            break;
        }
    }


    // Если пустой строки нет
    if (numEmptyString == STRING_IN_CONSOLE)
    {
        // То сдвигаем все строки на одну
        for (int i = 1; i < STRING_IN_CONSOLE; i++)
        {
            strcpy(&bufferConsole[i - 1][0], &bufferConsole[i][0]);
        }
        numEmptyString = STRING_IN_CONSOLE - 1;
    }

    char tempBuffer[1000] = "";

    static int numLines = 0;

    sprintf(tempBuffer, "%d %s", numLines++, string);

    if (strlen(tempBuffer) > SYMBOLS_IN_STRING - 1)
    {
        tempBuffer[SYMBOLS_IN_STRING - 1] = 0;
    }
    
    strcpy(&bufferConsole[numEmptyString][0], tempBuffer);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Display::DrawConsole(void)
{
    if (!CONSOLE_ENABLED)
    {
        return;
    }

    int y = 1;
    for (int i = 0; i < stringsInConsole; i++)
    {
        int length = Font::GetLengthText(&bufferConsole[i][0]);
        Painter::FillRegion(2, y, length, 8, Color::BACK);
        Text::DrawText(2, y, &bufferConsole[i][0], Color::FILL);
        y += 8;
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

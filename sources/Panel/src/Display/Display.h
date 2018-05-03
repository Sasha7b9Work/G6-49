#pragma once
#include "defines.h"
#include "Hardware/Controls.h"
#include "Settings/SettingsTypes.h"
#include "DisplayTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DISPLAY_ADD_STRING(x)       Display::AddStringToIndicating(x)
#define DISPLAY_SHOW_WARNING(warn)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Display
{
public:
    static void Init();

    static void Update();

    static uint8 *GetBuffer();

    static void SetButton(PanelControl control);

    static void AddStringToConsole(char *string);

    static void AddStringToIndicating(pString string);

private:
    static void DrawSignal(Channel chan);

    static void DrawSignalUGO(Channel chan, int y0);

    static void DrawSignalParameters(Channel chan, int y0);

    static void DrawParameterValue(Channel chan, WaveParameter parameter, int x, int y);

    static void DrawConsole();

    static const char *text;

    static const int STRING_IN_CONSOLE = 29;

    static const int SYMBOLS_IN_STRING = 64;

    static char bufferConsole[STRING_IN_CONSOLE][SYMBOLS_IN_STRING];
    // Экран
    static uint8 frontBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
    // Задний буфер. В нём происходит отрисовка, и затем изображение копируется во frontBuffer
    static uint8 backBuffer[SCREEN_WIDTH * SCREEN_HEIGHT];
};

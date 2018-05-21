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

    static void AddStringToConsole(char *string);

    static void AddStringToIndicating(pString string);

private:
    static void DrawSignal(Channel chan);

    static void DrawSignalUGO(Channel chan, int y0);

    static void DrawSignalParameters(Channel chan, int y0);

    static void DrawParameterValue(Channel chan, WaveParameter parameter, int x, int y);

    static void DrawConsole();

    static void DrawFrequencyCounter();

    static void ShowStatistics();
    /// Ќаписать подсказку
    static void DrawHint();

    static const int STRING_IN_CONSOLE = 29;

    static const int SYMBOLS_IN_STRING = 64;

    static char bufferConsole[STRING_IN_CONSOLE][SYMBOLS_IN_STRING];
    /// Ёкран
    static uint8 *frontBuffer;
    /// «адний буфер. ¬ нЄм происходит отрисовка, и затем изображение копируетс€ во frontBuffer
    static uint8 *backBuffer;
    /// ¬рем€, потраченное на отрисовку одного кадра
    static int timeFrame;
    /// «десь будем накапливать общее врем€ отрисовки
    static uint timeAccumFrames;
    /// ¬рем€ отрисовки всех кадров за 1 секунду
    static int timeAllFrames;
    /// ¬рем€ начала очередного 1-секундного отрезка, в течение которого замер€етс€ общее врем€ отрисовки
    static uint timeStartFrames;
};

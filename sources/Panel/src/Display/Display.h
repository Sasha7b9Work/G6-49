#pragma once
#include "defines.h"
#include "Hardware/Controls.h"
#include "Settings/SettingsTypes.h"
#include "DisplayTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DISPLAY_SHOW_WARNING(warn)

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Display
{
public:
    static void Init();

    static void Update();

    static uint8 *GetBuffer();

    static void AddStringToConsole(char *string);

private:
    static void DrawSignal(Channel chan);

    static void DrawSignalUGO(Channel chan, int y0);

    static void DrawSignalParameters(Channel chan, int y0);

    static void DrawParameterValue(Channel chan, WaveParameter parameter, int x, int y);

    static void DrawConsole();

    static void DrawFrequencyCounter();

    static void ShowStatistics();
    /// Написать подсказку
    static void DrawHint();

    static const int STRING_IN_CONSOLE = 29;

    static const int SYMBOLS_IN_STRING = 64;
    /// Здесь хранятся строки консоли
    static char bufferConsole[STRING_IN_CONSOLE][SYMBOLS_IN_STRING];
    /// Количество строк в консоли
    static int stringsInConsole;
    /// Экран
    static uint8 *frontBuffer;
    /// Задний буфер. В нём происходит отрисовка, и затем изображение копируется во frontBuffer
    static uint8 *backBuffer;
    /// Время, потраченное на отрисовку одного кадра
    static int timeFrame;
    /// Здесь будем накапливать общее время отрисовки
    static uint timeAccumFrames;
    /// Время отрисовки всех кадров за 1 секунду
    static int timeAllFrames;
    /// Время начала очередного 1-секундного отрезка, в течение которого замеряется общее время отрисовки
    static uint timeStartFrames;
};

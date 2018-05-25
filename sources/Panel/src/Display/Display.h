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

private:
    static void DrawFrequencyCounter();

    static void ShowStatistics();
    /// Ќаписать подсказку
    static void DrawHint();
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

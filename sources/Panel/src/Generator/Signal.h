#pragma once
#include "Settings/SettingsTypes.h"
#include "Settings/Settings.h"


class Signal
{
public:
    /// Нарисовать информацию о параметрах сигнала на экране
    static void Draw(Chan ch);
    /// Позиция x области отрисовки сигнала
    static int X();
    /// Позиция y области отрисовки сигнала
    static int Y(Chan ch);
    /// Ширина области отрисовки сигнала
    static int Width();
    /// Высота области отрисовки сигнала
    static int Height();
private:
    
    static void DrawSignalUGO(Chan chan, int y0);

    static void DrawSignalParameters(Chan chan, int y0);

    static void DrawParameterValue(Chan chan, Wave::Parameter parameter, int x, int y);
};

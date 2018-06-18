#pragma once
#include "Settings/SettingsTypes.h"


class Wave
{
public:
    /// Нарисовать информацию о параметрах сигнала на экране
    static void DrawSignal(Channel ch);
    /// Позиция x области отрисовки сигнала
    static int X();
    /// Позиция y области отрисовки сигнала
    static int Y(Channel ch);
    /// Ширина области отрисовки сигнала
    static int Width();
    /// Высота области отрисовки сигнала
    static int Height();
private:

    static void DrawSignalUGO(Channel chan, int y0);

    static void DrawSignalParameters(Channel chan, int y0);

    static void DrawParameterValue(Channel chan, Type_WaveParameter parameter, int x, int y);
};

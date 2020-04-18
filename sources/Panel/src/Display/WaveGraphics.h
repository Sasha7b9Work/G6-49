#pragma once
#pragma once
#include "Settings/SettingsTypes.h"


class Parameter;


class WaveGraphics
{
public:
    // Нарисовать информацию о параметрах сигнала на экране
    static void Draw(Chan::E ch);
    // Позиция x области отрисовки сигнала
    static int X();
    // Позиция y области отрисовки сигнала
    static int Y(Chan::E ch);
    // Ширина области отрисовки сигнала
    static int Width();
    // Высота области отрисовки сигнала
    static int Height();
private:

    static void DrawParameters(Chan::E chan, int y0);

    static void DrawParameterValue(Parameter *parameter, int x, int y);
};

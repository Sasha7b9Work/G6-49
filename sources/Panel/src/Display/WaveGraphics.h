#pragma once
#pragma once
#include "Settings/SettingsTypes.h"


struct Parameter;


namespace WaveGraphics
{
    // Нарисовать информацию о параметрах сигнала на экране
    void Draw(const Chan &);

    // Позиция x области отрисовки сигнала
    int X();

    // Позиция y области отрисовки сигнала
    int Y(const Chan &);

    // Ширина области отрисовки сигнала
    int Width();

    // Высота области отрисовки сигнала
    int Height();

    Parameter *GetParameterForDraw(const Chan &, int i);
};

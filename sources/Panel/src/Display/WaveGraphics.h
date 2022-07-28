#pragma once
#pragma once
#include "Settings/SettingsTypes.h"

class Parameter;


class WaveGraphics
{
public:
    
    // Нарисовать информацию о параметрах сигнала на экране
    static void Draw(const Chan &);
    
    // Позиция x области отрисовки сигнала
    static int X();
    
    // Позиция y области отрисовки сигнала
    static int Y(const Chan &);
    
    // Ширина области отрисовки сигнала
    static int Width();
    
    // Высота области отрисовки сигнала
    static int Height();


    static Parameter *GetParameterForDraw(const Chan &, int i);

private:

    static void DrawParameters(const Chan &, int y0);

    // Нарисовать УГО сигнала
    static void DrawUGO(const Chan &, int x, int y);
};

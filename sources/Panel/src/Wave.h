#pragma once
#include "Settings/SettingsTypes.h"


class Wave
{
public:
    /// Нарисовать информацию о параметрах сигнала на экране
    static void DrawSignal(Channel ch);
private:

    static void DrawSignalUGO(Channel chan, int y0);

    static void DrawSignalParameters(Channel chan, int y0);

    static void DrawParameterValue(Channel chan, Type_WaveParameter parameter, int x, int y);
};

#pragma once
#include "defines.h"


class FrequencyMeter
{
public:
    static void Draw();
    /// Установить выводимое значение частоты
    static void SetMeasure(uint value);

private:

    static uint frequency;
};

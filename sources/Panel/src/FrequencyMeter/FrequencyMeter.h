#pragma once
#include "defines.h"


class FrequencyMeter
{
public:
    static void Draw();
    /// Установить выводимое значение частоты
    static void SetMeasure(uint value);
    /// Этот метод вызывают для того, чтобу указать, что показания индикатора неактивны.
    static void SetInactive();

private:

    static uint frequency;

    static bool inactive;
};

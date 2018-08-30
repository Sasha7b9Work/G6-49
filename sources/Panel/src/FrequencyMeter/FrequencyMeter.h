#pragma once
#include "defines.h"

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)
#endif


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

public:
    struct BillingTime
    {
        enum E
        {
            _1ms,
            _10ms,
            _100ms,
            _1000ms,
            _10000ms
        } value;
        operator uint8() const { return (uint8)value; };
    };
};

#ifdef WIN32
#pragma warning(pop)
#endif

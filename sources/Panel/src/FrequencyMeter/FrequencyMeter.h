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
    /// ”становить выводимое значение частоты
    static void SetMeasure(uint value);
    /// Ётот метод вызывают дл€ того, чтобу указать, что показани€ индикатора неактивны.
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

    /// „исло усредн€емых период в режиме измерени€ периода сигнала
    struct AvePeriod
    {
        enum E
        {
            _1,
            _10,
            _100,
            _1000,
            _10000
        } value;
        operator uint8() const { return (uint8)value; };
    };
};

#ifdef WIN32
#pragma warning(pop)
#endif

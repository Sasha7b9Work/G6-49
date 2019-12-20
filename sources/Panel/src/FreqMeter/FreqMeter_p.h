#pragma once
#include "defines.h"


struct PFreqMeter
{
    /// Загрузить настройки в частотомер
    static void LoadSettings();

    static void Draw();
    /// Установить выводимое значение частоты
    static void SetMeasure(uint value);
    /// Этот метод вызывают для того, чтобу указать, что показания индикатора неактивны.
    static void SetInactive();
    /// Заслать в ПЛИС значение уровня
    static void LoadLevel();
    /// Заслать в ПЛИС значение гистерезиса
    static void LoadHysteresis();

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
        operator uint8() const { return static_cast<uint8>(value); };
    };

    /// Число усредняемых период в режиме измерения периода сигнала
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
        operator uint8() const { return static_cast<uint8>(value); };
    };
};

// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace PFreqMeter
{
    // Загрузить настройки в частотомер
    void LoadSettings();

    void Draw();
    
    // Установить выводимое значение частоты
    void SetMeasure(uint value);
    
    // Этот метод вызывают для того, чтобу указать, что показания индикатора неактивны.
    void SetInactive();
    
    // Заслать в ПЛИС значение уровня
    void LoadLevel();

    // Заслать в ПЛИС значение гистерезиса
    void LoadHysteresis();

    // Послать одно измерение в SCPI
    void SendMeasureToSCPI();

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

        BillingTime(E v) : value(v) { }

        operator uint8() const { return (uint8)value; };
    };

    // Число усредняемых период в режиме измерения периода сигнала
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

        AvePeriod(E v) : value(v) { }

        operator uint8() const { return (uint8)value; };
    };
};

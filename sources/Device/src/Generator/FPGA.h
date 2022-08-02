// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Generator/Generator_d.h"
#include "common/Command.h"


namespace FPGA
{
    static const int NUM_POINTS = 1024 * 8;

    void Init();

    void SetWaveForm(const Chan &, TypeForm::E);

    void SetFrequency(const Chan &);

    void SetAmplitude();

    void SetDurationImpulse(const Chan &, const Value &duration);

    void SetPeriodImpulse(const Chan &, const Value &period);

    // 1 - положительная полярность, 0 - отрицательная полярность
    void SetPolarity(const Chan &, uint8 polarity);

    // Установка режима запуска для произвольного (0) сигнала и импульсного (1) сигнала signal
    void SetStartMode(const Chan &, uint8 signal, StartMode::E);

    void EnableStartStopMode(StartStopMode::E);

    // Делает однократный запуск
    void SingleStart();

    void TransformCodeToData(const uint8 codeIn[FPGA::NUM_POINTS * 2], float dataOut[FPGA::NUM_POINTS]);

    struct PacketImpulse
    {
        // Устанавливает число импульсов в пачке
        static void SetNumberImpules(uint n);

        // Устанавливает период следования пачки
        static void SetPeriodPacket(Value period);

        static Value periodImpulse;

        static Value durationImpulse;
    };

    struct ModeWork
    {
        enum E
        {
            None,
            DDS,            // Работает по загруженным точкам заранее рассчитанных сигналов - пила, треугольник
            Impulse,        // Режим, в котором импульcы могут иметь разную частоту
            Impulse2,       // Режим, в котором импульсы имеют одинаковую частоту. При этом можно регулировать задержку второго канала отн. первого
            Rectangle,
            Meander,
            PackedImpulse,  // Пакеты импульсов
            Sine,
            Free,
            Count
        };
    };

    // Возвращает указатель на точки сигнала, загружаемого из флешки
    uint8 *DataFreeSignal(const Chan &);

    // Сохранить данные сигнала, загруженного с флешки
    void SaveExtSignal(const Chan &, uint8 *data);

    // Возвращает указатель на точки произвольного сигнала (программно определёного)
    uint8 *DataDDS(const Chan &);

    // Режим работы ПЛИС
    extern ModeWork::E modeWork[Chan::Count];

    static inline ModeWork::E CurrentMode(const Chan &ch) { return modeWork[ch]; }

    // Тактовая частота формирователя произвольных сигналов
    namespace ClockFrequencyAD992
    {
        enum E
        {
            _100MHz,
            _1MHz
        };

        extern E value;

        void Set(E);
    };
    
    // Тактовая частота формирователя импульсов
    struct ClockFrequencyImpulse
    {
        enum E
        {
            _100MHz,
            _1MHz
        };
    };
    
    extern ClockFrequencyImpulse::E clockImpulse;

    namespace Register
    {
        // Регистры ПЛИС
        enum E
        {
            _0_Control,
            _1_Freq,
            _2_Amplitude,
            _3_RectA,               // \brief Регистр кода сигнала прямоугольной формы канала А. Используется для установки двух кодов. 1-й код опреде-
                                    // ляет нижнее значение прямоугольного сигнала - смещение. 2-й код устанавливает верхнее значение сигнала - как бы
                                    // амплитуду.
                                    // b0...b13 - нижнее значение, b14...b27 - верхнее значение
            _4_RectB,               // Аналог _3_RectA для канала B
            _5_PeriodImpulseA,      // Период импульсов канала А
            _6_DurationImpulseA,    // Длительность импульсов канала А
            _7_PeriodImpulseB,      // Период импульсов канала В
            _8_DurationImpulseB,    // Длительность импульсов канала В
            _9_FreqMeter,
            _10_Offset,
            _11_Start,
            Count
        };
        // Записать значение в регистр
        void Write(E reg, uint64 value);

        uint64 Read(E reg);
    }
};

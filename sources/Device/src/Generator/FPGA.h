#pragma once
#include "Generator/Generator_d.h"
#include "Command.h"
#include "GeneratorSettingsTypes.h"




namespace FPGA
{
    static const uint NUM_POINTS = 1024 * 8;

    void Init();

    void SetWaveForm(Chan::E ch, Form::E form);

    void SetFrequency(Chan::E ch, ParamValue frequency);

    void SetAmplitude(Chan::E ch, ParamValue amplitude);

    void SetOffset(Chan::E ch, ParamValue offset);

    void SetDurationImpulse(Chan::E ch, ParamValue duration);

    void SetPeriodImpulse(Chan::E ch, ParamValue period);

    void SetPolarity(Chan::E ch, uint8 polarity);

    void SetStartMode(Chan::E ch, StartMode mode);
    /// Делает однократный запуск. Возвращает true в случае успеха (если установлены соответствующие настройки)
    bool Start();

    namespace PacketImpulse
    {
        /// Устанавливает число импульсов в пачке
        void SetNumberImpules(uint n);
        /// Устанавливает период следования пачки
        void SetPeriodPacket(ParamValue period);

        extern ParamValue periodImpulse;

        extern ParamValue durationImpulse;
    };

    struct ModeWork
    {
        enum E
        {
            None,
            DDS,
            Impulse,        ///< Режим, в котором импульcы могут иметь разную частоту
            Impulse2,       ///< Режим, в котором импульсы имеют одинаковую частоту. При этом можно регулировать задержку второго канала отн. первого
            Rectangle,
            Meander,
            PackedImpulse,  ///< Пакеты импульсов
            Sine,
            Count
        } value;
        explicit ModeWork(E v) : value(v) {};
    };

    struct ClockFrequency
    {
        enum E
        {
            _100MHz,
            _1MHz
        } value;
        explicit ClockFrequency(E v) : value(v) {};
    };

    ///< Регистры ПЛИС
    struct RG
    {
        enum E
        {
            _0_Control,
            _1_Freq,
            _2_Amplitude,
            _3_RectA,           /// \brief Регистр кода сигнала прямоугольной формы канала А. Используется для установки двух кодов. 1-й код опреде-
                                /// ляет нижнее значение прямоугольного сигнала - смещение. 2-й код устанавливает верхнее значение сигнала - как бы
                                /// амплитуду.
                                /// b0...b13 - нижнее значение, b14...b27 - верхнее значение
            _4_RectB,           ///< Аналог _3_RectA для канала B
            _5_PeriodImpulseA,
            _6_DurationImpulseA,
            _7_PeriodImpulseB,
            _8_DurationImpulseB,
            _9_FreqMeter,
            _10_Offset,
            _11_Start,
            Count
        } value;
        explicit RG(E v) : value(v) { };
    };

    void SetClockAD992(ClockFrequency::E clock);

    extern ClockFrequency::E clock;
    /// Режим работы ПЛИС
    extern ModeWork::E modeWork[Chan::Count];

    inline ModeWork::E CurrentMode(Chan::E ch) { return modeWork[ch]; }
    /// Возвращает указатель на точки сигнала, загружаемого из флешки
    uint8 *DataFlash(Chan::E ch);
    /// Сохранить данные сигнала, загруженного с флешки
    void SaveExtSignal(Chan::E ch, uint8 *data);
    /// Возвращает указатель на точки произвольного сигнала (программно определёного)
    uint8 *DataDDS(Chan::E ch);
    /// Записать значение в регистр
    void WriteRegister(RG::E reg, uint64 value);
};

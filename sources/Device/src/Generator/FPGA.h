#pragma once
#include "defines.h"
#include "Command.h"
#include "GeneratorSettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FPGA_NUM_POINTS (1024 * 8)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FPGA
{
friend class Interface;

public:
    static void Init();

    static void SetWaveForm(Chan ch, Form form);

    static void SetFrequency(Chan ch, float frequency);

    static void SetAmplitude(Chan ch, float amplitude);

    static void SetOffset(Chan ch, float offset);

    static void SetDurationImpulse(Chan ch, float duration);

    static void SetPeriodImpulse(Chan ch, float period);

    static void SetStartMode(Chan ch, StartMode mode);

    class PacketImpulse
    {
    public:
        /// Устанавливает число импульсов в пачке
        static void SetNumberImpules(uint n);
        /// Устанавливает период следования пачки
        static void SetPeriodPacket(float period);
        /// Устанавливает длительность импульса
        static void SetDurationImpulse(float duration);
        /// Устанавливает период следования импульсов в пачке
        static void SetPeriodImpulse(float period);

        static float periodImpulse;

        static float durationImpulse;
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
            Manipulation,   ///< Режим амплитудной манипуляции
            PackedImpulse,  ///< Пакеты импульсов
            Number
        } value;
        ModeWork(E v) : value(v) {};
        operator uint8() const { return(uint8)value; };
    };

    static struct ClockFrequency
    {
        enum E
        {
            _100MHz,
            _1MHz
        } value;
        ClockFrequency(E v) : value(v) {};
        operator uint8() const { return (uint8)value; };
    } clock;

private:

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
            Number
        } value;
        RG(E v) : value(v) { };
        operator uint8() const { return (uint8)value; };
    };

    struct RG0
    {
        enum E
        {
            _0_WriteData,       ///< В этот бит записываем 0, перед загрузкой данных сигнала в ПЛИС
            _1_ImpulseA,        ///< 1, если в канале А ПЛИС формирует импульсы/прямоугольник
            _2_ImpulseB,        ///< 1, если в канале B ПЛИС формирует импульсы/прямоугольник
            _3_ManipulationB,   ///< Здесь 0, если включена манипуляция канала B
            _4,
            _5,
            _6_ManipulationA,   ///< 0, если включена манипуляция в канале A
            _7_Freq_MHz,        ///< 1, если тактовая частота 1МГц
            _8_MeanderA,        ///< 1, если меандр по каналу A
            _9_MeanderB,        ///< 1, если меандр по каналу B
            _10_HandStartA,     ///< Если бит установлен в 1, то ручной режим запуска
            _11_HandStartB,
            _12_HandStartPacket ///< 1, если включён пакетный режим импульсов
        };
    };

    static void SetManipulationMode(Chan ch);

    static void SetRampPlusMode(Chan ch);

    static void SetRampMinusMode(Chan ch);

    static void SetMeanderMode(Chan ch);

    static void SetImpulseMode(Chan ch);

    static void SetPackedImpulseMode(Chan ch);

    static void CreateEnable();
    /// Заслать рассчитанные точки в плис
    static void SendData();
    /// Записать байт в ПЛИС
    static void WriteByte(uint8 byte);
    /// Записать значение в регистр
    static void WriteRegister(uint8 reg, uint64 value);
    /// Установить на A0_RG...A3_RG адрес, соответсвующй регистру
    static void WriteAddress(uint8 reg);

    static void WriteControlRegister();
    /// Преобразует данные, записанные в относительных единицах [-1.0f;1.0f] в данные, записанные в прямом коде, пригодные для отправки в ПЛИС
    static void TransformDataToCode(float data[FPGA_NUM_POINTS], uint8 code[FPGA_NUM_POINTS * 2]);

    static uint8 RegisterForDuration(Chan ch);
    /// Преобразует смещение в прямой код, пригодный для записи в альтеру
    static uint OffsetToCode(float offset);
    /// Если true - автоматический режим запуска (по умолчанию)
    static bool autoStart[Chan::Number];
    /// Режим работы ПЛИС
    static ModeWork modeWork[Chan::Number];
    /// \brief Здесь хранятся значения, предназначенные непосредственно для засылки в ПЛИС. Сначала идут младшие 8 бит, а потом старшие 6 бит
    /// Данные должны быть записаны в прямом коде - 0 в старшем разряде обозначает положительное число, а 1 - отрицательное
    static uint8 dataDDS[Chan::Number][FPGA_NUM_POINTS * 2];

    static float amplitude[Chan::Number];

    static float offset[Chan::Number];
    /// Здесь хранятся записанные в регистры значения
    static uint64 registers[RG::Number];
};

#pragma once
#include "defines.h"
#include "Command.h"
#include "GeneratorSettingsTypes.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FPGA_NUM_POINTS (1024 * 8)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FPGA
{
friend class Interface;
friend class Handlers;

public:
    static void Init();

    static void SetWaveForm(Chan::E ch, Form form);

    static void SetFrequency(Chan::E ch, ParamValue frequency);

    static void SetAmplitude(Chan::E ch, ParamValue amplitude);

    static void SetOffset(Chan::E ch, ParamValue offset);

    static void SetDurationImpulse(Chan::E ch, ParamValue duration);

    static void SetPeriodImpulse(Chan::E ch, ParamValue period);

    static void SetPolarity(Chan::E ch, uint8 polarity);

    static void SetStartMode(Chan::E ch, StartMode mode);
    /// Делает однократный запуск. Возвращает true в случае успеха (если установлены соответствующие настройки)
    static bool Start();

    class PacketImpulse
    {
    public:
        /// Устанавливает число импульсов в пачке
        static void SetNumberImpules(uint n);
        /// Устанавливает период следования пачки
        static void SetPeriodPacket(ParamValue period);

        static ParamValue periodImpulse;

        static ParamValue durationImpulse;
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
            Number
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

    static ClockFrequency::E clock;

    static ModeWork::E CurrentMode(Chan::E ch) { return modeWork[ch]; }

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
            _11_Start,
            Number
        } value;
        RG(E v) : value(v) { };
        operator uint8() const { return (uint8)value; };
    };

    struct RG0
    {
        enum E
        {
            _0_WriteData,           ///< В этот бит записываем 0, перед загрузкой данных сигнала в ПЛИС
            _1_ImpulseA,            ///< 1, если в канале А ПЛИС формирует импульсы/прямоугольник
            _2_ImpulseB,            ///< 1, если в канале B ПЛИС формирует импульсы/прямоугольник
            _3_ManipulationOSK2,    ///< Здесь 0, если синус канала 1 должен манипулироваться сигналом OSK2 ("пилой" от AD9952 второго канала)
            _4_ManipulationFPGA1,   ///< Здесь 0, если синус канала 1 должен манипулироваться формирователем импульсов канала 1
            _5_ManipulationOSK1,    ///< Здесь 0, если синус канала 2 должен манипулироваться сигналом OSK1 ("пилой" от AD9952 первого канала)
            _6_ManipulationFPGA2,   ///< Здесь 0, есил синус канала 2 должен манипулироваться формирователем импульсов канала 2
            _7_Freq_MHz,            ///< 1, если тактовая частота 1МГц
            _8_MeanderA,            ///< 1, если меандр по каналу A
            _9_MeanderB,            ///< 1, если меандр по каналу B
            _10_HandStartA,         ///< Если бит установлен в 1, то ручной режим запуска
            _11_HandStartB,
            _12_HandStartPacket,    ///< 1, если включён пакетный режим импульсов
            _13_StartMode0,         ///< Младший бит управления режимом запуска
            _14_StartMode1          ///< Старший бит управления режимом запуска
        };
    };

    class Multiplexor
    {
        friend class Interface;
    public:

        /// Первоначальная инициализация
        static void Init();
        /// Скоммутировать мультиплексор в соответствии с формой устанавливаемого сигнала на данном канале
        static void SetMode(Chan::E ch, Form form);

        static Form GetMode(Chan ch);

    private:
        static void SetPin(uint16 pin);
        static void ResetPin(uint16 pin);

        static void WriteRegister(Register reg, uint value);

        static Form mode[Chan::Number];
    };

    static void SetModeSine(Chan::E ch);
    /// Установить режим произвольного сигнала по каналу ch
    static void SetModeDDS(Chan::E ch);
    /// Возвращает указатель на точки произвольного сигнала
    static uint8 *DataDDS(Chan::E ch);

    static void SetModeMeander(Chan::E ch);

    static void SetModeImpulse(Chan::E ch);

    static void SetModePackedImpulse(Chan::E ch);

    static void EmptyFunc(Chan ch);

    static void CreateEnable();
    /// Заслать рассчитанные точки в плис
    static void SendData();
    /// Записать байт в ПЛИС
    static void WriteByte(uint8 byte);
    /// Записать значение в регистр
    static void WriteRegister(uint8 reg, uint64 value);
    /// Установить на A0_RG...A3_RG адрес, соответсвующй регистру
    static void WriteAddress(uint8 reg);
    /// Запись управляющего регистра
    static void WriteControlRegister();
    /// Преобразует данные, записанные в относительных единицах [-1.0f;1.0f] в данные, записанные в прямом коде, пригодные для отправки в ПЛИС
    static void TransformDataToCode(float data[FPGA_NUM_POINTS], uint8 code[FPGA_NUM_POINTS * 2]);
    /// Записывает коды, соответствующие максимальному и минимальному значению
    static void WriteMaxAmplitude(Chan::E ch);

    static uint8 RegisterForDuration(Chan ch);
    /// Преобразует смещение в прямой код, пригодный для записи в альтеру
    static uint OffsetToCode(float offset);
    /// Установить биты, соответствующие режиму запуска
    static uint16 SetBitsStartMode(uint16 data);
    /// Режим запуска
    static StartMode startMode[Chan::Number];
    /// Режим работы ПЛИС
    static ModeWork::E modeWork[Chan::Number];
    /// \brief Здесь хранятся значения, предназначенные непосредственно для засылки в ПЛИС. Сначала идут младшие 8 бит, а потом старшие 6 бит
    /// Данные должны быть записаны в прямом коде - 0 в старшем разряде обозначает положительное число, а 1 - отрицательное
    static uint8 dataDDS[Chan::Number][FPGA_NUM_POINTS * 2];

    static float amplitude[Chan::Number];

    static float offset[Chan::Number];
    /// Здесь хранятся записанные в регистры значения
    static uint64 registers[RG::Number];
};

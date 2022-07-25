// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Generator/Generator_d.h"
#include "common/Command.h"


struct FPGA
{
    static const int NUM_POINTS = 1024 * 8;

    static void Init();

    static void SetWaveForm(Chan::E ch, TypeForm::E form);

    static void SetFrequency(Chan::E ch);

    static void SetAmplitude();

    static void SetDurationImpulse(Chan::E ch, Value duration);

    static void SetPeriodImpulse(Chan::E ch, Value period);

    // 1 - положительная полярность, 0 - отрицательная полярность
    static void SetPolarity(Chan::E ch, uint8 polarity);

    // Установка режима запуска для произвольного (0) сигнала и импульсного (1) сигнала signal
    static void SetStartMode(Chan::E ch, uint8 signal, StartMode mode);
    
    // Делает однократный запуск
    static void SingleStart();

    static void TransformCodeToData(const uint8 codeIn[FPGA::NUM_POINTS * 2], float dataOut[FPGA::NUM_POINTS]);

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

    // Регистры ПЛИС
    struct RG
    {
        enum E
        {
            _0_Control,
            _1_Freq,
            _2_Amplitude,
            _3_RectA,           // \brief Регистр кода сигнала прямоугольной формы канала А. Используется для установки двух кодов. 1-й код опреде-
                                // ляет нижнее значение прямоугольного сигнала - смещение. 2-й код устанавливает верхнее значение сигнала - как бы
                                // амплитуду.
                                // b0...b13 - нижнее значение, b14...b27 - верхнее значение
            _4_RectB,           // Аналог _3_RectA для канала B
            _5_PeriodImpulseA,      // 
            _6_DurationImpulseA,    // Сюда же записывается количество
            _7_PeriodImpulseB,
            _8_DurationImpulseB,
            _9_FreqMeter,
            _10_Offset,
            _11_Start,
            Count
        } value;
        explicit RG(E v) : value(v) { };
    };

    static void SetClockAD992(ClockFrequency::E clock);

    static ClockFrequency::E clock;
    
    // Режим работы ПЛИС
    static ModeWork::E modeWork[Chan::Count];

    static inline ModeWork::E CurrentMode(Chan::E ch) { return modeWork[ch]; }
    
    // Возвращает указатель на точки сигнала, загружаемого из флешки
    static uint8 *DataFreeSignal(Chan::E ch);
    
    // Сохранить данные сигнала, загруженного с флешки
    static void SaveExtSignal(Chan::E ch, uint8 *data);
    
    // Возвращает указатель на точки произвольного сигнала (программно определёного)
    static uint8 *DataDDS(Chan::E ch);
    
    // Записать значение в регистр
    static void WriteRegister(RG::E reg, uint64 value);

private:
    struct RG0
    {
        enum E
        {
            _0_WriteData,           // В этот бит записываем 0, перед загрузкой данных сигнала в ПЛИС
            _1_ImpulseA,            // 1, если в канале А ПЛИС формирует импульсы/прямоугольник
            _2_ImpulseB,            // 1, если в канале B ПЛИС формирует импульсы/прямоугольник
            _3_ManipulationOSK2,    // Здесь 0, если синус канала 1 должен манипулироваться сигналом OSK2 ("пилой" от AD9952 второго канала)
            deleted__4_ManipulationFPGA1,   // Здесь 0, если синус канала 1 должен манипулироваться формирователем импульсов канала 1
            _5_ManipulationOSK1,    // Здесь 0, если синус канала 2 должен манипулироваться сигналом OSK1 ("пилой" от AD9952 первого канала)
            deleted_6_ManipulationFPGA2,   // Здесь 0, есил синус канала 2 должен манипулироваться формирователем импульсов канала 2
            _7_Freq_MHz,            // 1, если тактовая частота 1МГц
            _8_MeanderA,            // 1, если меандр по каналу A
            _9_MeanderB,            // 1, если меандр по каналу B
            _10_HandStartA,         // Если бит установлен в 1, то ручной режим запуска
            _11_HandStartB,
            _12_PacketImpulse,      // 1, если включён пакетный режим импульсов
            _13_StartMode0,         // Младший бит управления режимом запуска
            _14_StartMode1          // Старший бит управления режимом запуска
        };
    };
    
    static void SetFormSine(Chan::E ch);
    
    // Установить режим Пила+
    static void SetFormRampPlus(Chan::E ch);
    
    // Установить режим Пила-
    static void SetFormRampMinus(Chan::E ch);
    
    // Установить режим Треугольник
    static void SetFormTriangle(Chan::E ch);
    
    // Установить режим произвольного сигнала, загруженного с флешки
    static void SetFormFree(Chan::E ch);
    
    static void SetFormMeander(Chan::E ch);
    
    static void SetFormImpulse(Chan::E ch);
    
    static void SetFormPackedImpulse(Chan::E ch);
    
    // Заслать рассчитанные точки обоих каналов в плис
    static void SendData();

    // Заслать рассчитанные точки одного канала в плис
    static void SendDataChannel(Chan::E ch);
    
    // Установить на A0_RG...A3_RG адрес, соответсвующй регистру
    static void WriteAddress(RG::E reg);
    
    // Запись управляющего регистра
    static void WriteControlRegister();
    
    // Записывает коды, соответствующие максимальному и минимальному значению
    static void WriteMaxAmplitude(Chan::E ch);
    
    // Установить биты, соответствующие режиму запуска
    static void SetBitsStartMode(uint16 &data);
    static void SetBitsStartMode(Chan::E ch, uint16 &data);

    // Возвращает true, если по каналу ch работает DDS
    static bool InModeDDS(Chan::E ch);

    static StartMode startMode[Chan::Count][2];          // Режим запуска для произвольного сигнала (0) и для импульсного сигнала (1)
    
    static uint64 registers[RG::Count];     // Здесь хранятся записанные в регистры значения
};

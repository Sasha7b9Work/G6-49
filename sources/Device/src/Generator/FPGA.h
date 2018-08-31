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

    static void SetWaveForm(Wave::Form form);

    static void SetFrequency(Chan ch, float frequency);

    static void SetAmplitude(Chan ch, float amplitude);

    static void SetOffset(Chan ch, float offset);

private:
    enum ModeWorkFPGA
    {
        ModeNone,
        ModeDDS,
        ModeImpulse,    ///< Режим, в котором импульcы могут иметь разную частоту
        ModeImpulse2    ///< Режим, в котором импульсы имеют одинаковую частоту. При этом можно регулировать задержку второго канала отн. первого
    };

    struct RG
    {
        enum E
        {
            _0_Control,
            _1_Freq,
            _2_Amplitude,
            _3_RectA,
            _4_RectB,
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

    static void CreateSine();

    static void CreateRampPlus();

    static void CreateRampMinus();
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

    static ModeWorkFPGA modeWork;
    /// \brief Здесь хранятся значения, предназначенные непосредственно для засылки в ПЛИС. Сначала идут младшие 8 бит, а потом старшие 6 бит
    /// Данные должны быть записаны в прямом коде - 0 в старшем разряде обозначает положительное число, а 1 - отрицательное
    static uint8 dataA[FPGA_NUM_POINTS * 2];
    static uint16 dataB[FPGA_NUM_POINTS];

    static float amplitude[Chan::Number];

    static float offset[Chan::Number];
};

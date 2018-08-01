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

    static void SetWaveForm(Type_WaveForm form);

    static void SetFrequency(Channel ch, float frequency);

    static void SetDuration(Channel ch, float duration);

    static void SetDelay(float delay);

private:
    enum ModeWorkFPGA
    {
        ModeNone,
        ModeDDS,
        ModeImpulse,    ///< Режим, в котором импульcы могут иметь разную частоту
        ModeImpulse2    ///< Режим, в котором импульсы имеют одинаковую частоту. При этом можно регулировать задержку второго канала отн. первого
    };

    enum
    {
        RG0_Control,
        RG1_Freq,
        RG2_Mul,
        RG3_RectA,
        RG4_RectB,
        RG5_PeriodImpulseA,
        RG6_DurationImpulseA,
        RG7_PeriodImpulseB,
        RG8_DurationImpulseB,
        RG9_FreqMeter,
        NumRegistersFPGA
        /*
        Reg_Control,
        Reg_PeriodA,
        Reg_DurationA,
        Reg_PeriodB,
        Reg_DurationBandDelay,
        Reg_DurationB,
        Reg_Frequency
        */
    };

    static void CreateSine();

    static void CreateCosine();

    static void CreateMeander();

    static void CreateRampPlus();

    static void CreateRampMinus();

    static void CreateTriangle();

    static void CreateTrapeze();

    //static void GenerateSaw();
    /// Установить импульсный режим работы
    static void CreateImpulse();

    static void CreateExponentePlus();

    static void CreateExponenteMinus();

    static void CreateNoise();

    static void CreateFree();

    /// Заслать рассчитанные точки в плис
    static void SendData();
    /// Записать значение в регистр
    static void WriteRegister(uint8 reg, uint value);
    /// Установить на A0_RG...A3_RG адрес, соответсвующй регистру
    static void WriteAddress(uint8 reg);

    static void WriteControlRegister();

    static uint8 RegisterForDuration(Channel ch);

    static ModeWorkFPGA modeWork;

    static uint16 dataA[FPGA_NUM_POINTS];   ///< Точки сигнала для засылки в ПЛИС
    static uint16 dataB[FPGA_NUM_POINTS];
};

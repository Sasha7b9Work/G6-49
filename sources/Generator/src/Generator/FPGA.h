#pragma once
#include "defines.h"
#include "Command.h"
#include "GeneratorSettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FPGA_NUM_POINTS 1024


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FPGA
{
public:
    static void Init();

    static void SetWaveForm(WaveForm form);

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

    static void GenerateSine();

    static void GenerateSaw();
    /// Установить импульсный режим работы
    static void SetModeImpulse();
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

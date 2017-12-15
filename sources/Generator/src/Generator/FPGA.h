#pragma once
#include "defines.h"
#include "../../Common/Command.h"
#include "GeneratorSettingsTypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FPGA_NUM_POINTS 1024


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FPGA
{
public:
    FPGA();

    void Init();

    void SetWaveForm(WaveForm form);

    void SetFrequency(Channel ch, float frequency);

    void SetDuration(Channel ch, float duration);

    void SetDelay(float delay);

private:
    typedef enum
    {
        ModeNone,
        ModeDDS,
        ModeImpulse,    ///< Режим, в котором импульcы могут иметь разную частоту
        ModeImpulse2    ///< Режим, в котором импульсы имеют одинаковую частоту. При этом можно регулировать задержку второго канала отн. первого
    } ModeWorkFPGA;

    void GenerateSine();

    void GenerateSaw();
    /// Установить импульсный режим работы
    void SetModeImpulse();
    /// Заслать рассчитанные точки в плис
    void SendData();
    /// Записать значение в регистр
    void WriteRegister(uint8 reg, uint value);
    /// Установить на A0_RG...A3_RG адрес, соответсвующй регистру
    void WriteAddress(uint8 reg);

    void WriteControlRegister();

    uint8 RegisterForDuration(Channel ch);

    ModeWorkFPGA modeWork;

    uint16 dataA[FPGA_NUM_POINTS];   ///< Точки сигнала для засылки в ПЛИС
    uint16 dataB[FPGA_NUM_POINTS];
};

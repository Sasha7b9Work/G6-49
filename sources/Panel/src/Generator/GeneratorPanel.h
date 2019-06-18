#pragma once
#include "Settings/SettingsTypes.h"
#include "Command.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define START_MODE(ch)          (set.fpga_startMode[ch])
#define START_MODE_A            START_MODE(Chan::A)
#define START_MODE_B            START_MODE(Chan::B)
#define START_MODE_A_IS_SINGLE  (START_MODE_A == StartMode::Single)
#define START_MODE_B_IS_SINGLE  (START_MODE_B == StartMode::Single)

class SimpleMessage;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Generator
{
    static const int FPGA_NUM_POINTS = 8 * 1024;

    void Reset();

    void EnableChannel(Chan::E ch, bool enable);

    void SetFormWave(Wave *w);

    void SetFormWave(Chan::E ch, Form::E form);

    void SetParameter(ParameterValue *param);

    void SetParameter(ParameterChoice *param);

    void SetFrequency(Chan::E ch, float freq);

    void SetOffset(Chan::E ch, float offset);

    void SetAmplitude(Chan::E ch, float amplitude);

    void SetDebugMode(bool enable);

    void LoadRegister(Register::E reg, uint64 value);

    void LoadSettings();

    void TuneChannel(Chan::E ch);
    /// Загружает режим запуска из set в аппаратуру
    void LoadStartMode(Chan::E ch, int mode);
};

#pragma once
#include "Settings/CalibrationSettings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace EEPROM
{
    void Init();

    void SaveSettings(CalibrationSettings *settings);

    void LoadSettings(CalibrationSettings *settings);

    void SaveSignal(Chan::E ch, float *data);

    float *GetSignal(Chan::E ch);
};

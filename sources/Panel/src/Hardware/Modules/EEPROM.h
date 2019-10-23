#pragma once
#include "Settings/CalibrationSettings.h"



namespace EEPROM
{
    void SaveSettings(CalibrationSettings *settings);

    void LoadSettings(CalibrationSettings *settings);
};

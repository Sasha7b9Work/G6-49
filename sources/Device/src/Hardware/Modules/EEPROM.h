#pragma once
#include "Settings/CalibrationSettings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace EEPROM
{
    void Init();

    void SaveSettings(CalibrationSettings *settings);

    void LoadSettings(CalibrationSettings *settings);

    namespace Signal
    {
        void Save(Chan::E ch, uint16 data[Generator::DDS_NUM_POINTS]);

        uint16 *Get(Chan::E ch);
    }
};

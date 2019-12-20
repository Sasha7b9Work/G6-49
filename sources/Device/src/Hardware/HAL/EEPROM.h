#pragma once
#include "Settings/CalibrationSettings.h"
#include "Generator/Generator_d.h"



struct HAL_EEPROM
{
    static void Init();

    static void SaveSettings(CalibrationSettings *settings);

    static void LoadSettings(CalibrationSettings *settings);

    struct Signal
    {
        static void Save(Chan::E ch, uint16 data[DGenerator::DDS_NUM_POINTS]);

        static uint16 *Get(Chan::E ch);
    };
};

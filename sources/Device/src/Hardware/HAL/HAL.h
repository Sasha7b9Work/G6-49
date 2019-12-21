#pragma once
#include "Generator/Generator_d.h"


struct CalibrationSettings;


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


struct HAL_HCD
{
    static void Init();

    static HCD_HandleTypeDef handle;
};


struct HAL_SPI1
{
    static void Init();

    static bool Receive(void *buffer, uint size, uint timeout);

    static bool Transmit(void *buffer, uint size, uint timeout);

    static bool Transmit(uint value, uint timeout);
};

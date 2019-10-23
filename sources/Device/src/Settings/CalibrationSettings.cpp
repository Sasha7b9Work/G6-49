#include "stdafx.h"
#ifndef WIN32
#include "CalibrationSettings.h"
#include "Hardware/Modules/EEPROM.h"
#endif



static const CalibrationSettings defSet =
{
    0,              // Это значение не должно равняться (-1), чтобы загрузчик мог определить наличие настроек в EEPROM-памяти
    {4095, 4095},   // AD9952 -5В
    {2048, 2048},   // AD9952 0В
    {0,    0},      // AD9952 +5В
    {0,    0},      // AD9952 Размах
    {0,    0},      // DDS MAX
    {0,    0},      // DDS MIN
    {2048, 2048},      // DDS OFFSET
    0               // FREQ TRIG LEV
};

CalibrationSettings setCal = defSet;


CalibrationSettings &CalibrationSettings::operator =(const CalibrationSettings &rhs)
{
    std::memcpy(this, rhs.unused, sizeof(*this));

    return *this;
}


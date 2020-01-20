#include "CalibrationSettings.h"
#include <cstring>


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

static int16 *currentCAL = CalibrationSettings::GetK(0, 0, 0, 0);


CalibrationSettings &CalibrationSettings::operator =(const CalibrationSettings &rhs)
{
    std::memcpy(this, rhs.unused, sizeof(*this));

    return *this;
}


int16 *CalibrationSettings::GetK(uint8 channel, uint8 signal, uint8 range, uint8 parameter)
{
    if(channel > 1)
    {
        channel = 1;
    }

    if(signal > 1)
    {
        signal = 1;
    }

    currentCAL = &setCal.cal[channel][signal][range][parameter];

    return currentCAL;
}

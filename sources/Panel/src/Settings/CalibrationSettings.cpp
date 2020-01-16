#include "defines.h"
#include "log.h"
#include "structs.h"
#include "CalibrationSettings.h"
#include "Hardware/HAL/HAL.h"


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


void CalibrationSettings::Load()
{
    *this = defSet;                     // Сначала заполняем значениями по умолчанию - вдруг сохранённых настроек нету
    HAL_EEPROM::LoadSettings(this);

    for (int ch = 0; ch < Chan::Count; ch++)
    {
        for (int k = 0; k < KoeffCal::Count; k++)
        {
            Message::Calibrate(static_cast<uint8>(ch), static_cast<uint8>(k)).Transmit();
        }
    }
}


void CalibrationSettings::Save()
{
    HAL_EEPROM::SaveSettings(this);
}


CalibrationSettings &CalibrationSettings::operator =(const CalibrationSettings &rhs)
{
    std::memcpy(this, rhs.unused, sizeof(*this));

    return *this;
}


static int16 *currentCAL = CalibrationSettings::GetK(0, 0, 0, 0);



int16 *CalibrationSettings::GetK(int channel, int signal, int range, uint8 parameter)
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


void CalibrationSettings::ReduceK()
{
    if(*currentCAL > -1024)
    {
        *currentCAL = *currentCAL - 1;
    }
}


void CalibrationSettings::IncreaseK()
{
    if(*currentCAL < 1024)
    {
        *currentCAL = *currentCAL + 1;
    }
}

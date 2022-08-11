// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "structs.h"
#include "FreqMeter/FreqMeter_p.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/CalibrationSettings.h"


static const CalibrationSettings defCalSet =
{
    {0}
};

CalibrationSettings setCal = defCalSet;

static int16 *currentCAL = CalibrationSettings::GetK(0, 0, 0, 0);


void CalibrationSettings::Load()
{
    *this = defCalSet;                     // Сначала заполняем значениями по умолчанию - вдруг сохранённых настроек нету
    HAL_EEPROM::LoadSettings(this);

    for(int ch = 0; ch < NUM_CHAN; ch++)
    {
        for(int sig = 0; sig < NUM_SIGNALS; sig++)
        {
            for(int range = 0; range < NUM_RANGES; range++)
            {
                for(int param = 0; param < NUM_PARAMETERS; param++)
                {
                    Message::CalibrateLoad(
                        (uint8)ch,
                        (uint8)sig,
                        (uint8)range,
                        (uint8)param
                    ).Transmit();
                }
            }
        }
    }

    Message::SetFreqMeterTrigK(kFreqMeter).Transmit();
    PFreqMeter::LoadLevel();
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


void CalibrationSettings::ReduceK(int16 min)
{
    if(*currentCAL > min)
    {
        *currentCAL = *currentCAL - 1;
    }
}


void CalibrationSettings::IncreaseK(int16 max)
{
    if(*currentCAL < max)
    {
        *currentCAL = *currentCAL + 1;
    }
}

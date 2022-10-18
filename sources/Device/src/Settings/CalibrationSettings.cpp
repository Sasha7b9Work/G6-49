// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "CalibrationSettings.h"
#include <cstring>


static const CalibrationSettings defSet =
{
    {0}             // Это значение не должно равняться (-1), чтобы загрузчик мог определить наличие настроек в EEPROM-памяти
};

CalibrationSettings setCal = defSet;


CalibrationSettings &CalibrationSettings::operator =(const CalibrationSettings &rhs)
{
    std::memcpy(this, rhs.unused, sizeof(*this));

    return *this;
}


int16 *CalibrationSettings::GetK(uint8 channel, CalSignal::E signal, uint8 range, uint8 parameter)
{
    if(channel > 1)
    {
        channel = 1;
    }

    if (signal == CalSignal::Impulse && parameter == 0) // Для импульсного сигнала свой коэффициент амплитуды
    {
        return &setCal.cal_a_imp[channel];
    }

    if(signal > 1)
    {
        signal = CalSignal::DDS;
    }

    return &setCal.cal[channel][signal][range][parameter];
}


int16 *CalibrationSettings::GetFreqMeterK()
{
    return &setCal.kFreqMeterTrig;
}

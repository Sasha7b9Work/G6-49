// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "CalibrationSettings.h"
#include <cstring>


static const CalibrationSettings defSet =
{
    {0}             // ��� �������� �� ������ ��������� (-1), ����� ��������� ��� ���������� ������� �������� � EEPROM-������
};

CalibrationSettings setCal = defSet;


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

    return &setCal.cal[channel][signal][range][parameter];
}


int16 *CalibrationSettings::GetFreqMeterK()
{
    return &setCal.kFreqMeterTrig;
}

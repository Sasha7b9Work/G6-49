// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Messages_p.h"
#include "Settings/CalibrationSettings.h"


Message::LoadFormDDS::LoadFormDDS(uint8 ch, uint16 data[PGenerator::DDS_NUM_POINTS]) : SimpleMessage(2 + PGenerator::DDS_NUM_POINTS * 2, Command::LoadFormDDS)
{
    PutUINT8(ch);
    PutData((uint8*)data, sizeof(data[0]) * PGenerator::DDS_NUM_POINTS);
}


Message::CalibrateSet::CalibrateSet(uint8 ch, uint8 signal, uint8 range, uint8 parameter) : SimpleMessage(7, Command::CalibrationSet)
{
    PutUINT8(ch);
    PutUINT8(signal);
    PutUINT8(range);
    PutUINT8(parameter);
    PutINT16(*setCal.GetK(ch, signal, range, parameter));
}


Message::CalibrateLoad::CalibrateLoad(uint8 ch, uint8 signal, uint8 range, uint8 parameter) : SimpleMessage(7, Command::CalibrationLoad)
{
    PutUINT8(ch);
    PutUINT8(signal);
    PutUINT8(range);
    PutUINT8(parameter);
    PutINT16(*setCal.GetK(ch, signal, range, parameter));
}

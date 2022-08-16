// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Command.h"
#include "structs.h"
#include <cstdio>
#include <cstring>



cstr Command::Name() const
{
    static const cstr names[Count] =
    {
        "RequestData",
        "EnableChannel",
        "SetFormWave",
        "SetFrequency",
        "SetAmplitude",
        "SetOffset",
        "SetDuration",
        "SetPhase",
        "ModeDebug",
        "SetDelay",
        "WriteRegister",
        "SetDurationRise",
        "SetDurationFall",
        "SetDurationStady",
        "SetDutyFactor",
        "SetManipulation",
        "SetManipulationDuration",
        "SetManipulationPeriod",
        "SetPacketPeriod",
        "SetPacketNumber",
        "SetStartMode",
        "SetPeriod",
        "SetPolarity",
        "LoadFormDDS",
        "FreqMeasure",
        "Log",
        "FDrive_NumDirsAndFiles",
        "FDrive_Mount",
        "FDrive_RequestDir",
        "FDrive_RequestFile",
        "Test",
        "FDrive_RequestFileSize",
        "FDrive_RequestFileString",
        "FDrive_LoadDDSfromFile",
        "FDrive_RequestPictureDDSfromFile",
        "SCPI_Data",
        "PortCPU",
        "CalibrationLoad",
        "CalibrationSet",
        "StartApplication",
        "RequestUpgrade",
        "PortionUpgradeDevice",
        "AnswerUpgradePanel",
        "RequestPortionUpgradePanel",
        "AnswerPortionUpgradePanel",
        "Debug",
        "SetFreqMeterTrigK",
        "RequestPictureDDSfromStorage",
        "LoadDDSfromStorage",
        "FDrive_CreateFile",
        "FDrive_WriteToFile",
        "FDrive_CloseFile",
        "SingleStart",
        "StartStopMode",
        "SetClockImpulse"
    };

    if((int)value >= 0 && value < Count)
    {
        return names[value];
    }
    
    // \todo Здесь страшный баг - при большинстве других значений SIZE процессор виснет, даже не заходя в функцию Command::Trace()
    // Возможно, это происходит из-за использования CCM RAM
    const int SIZE = 23;
    static char buffer[SIZE + 1];
    std::snprintf(buffer, SIZE, "Неправильный параметр %d", value);

    return buffer;
}

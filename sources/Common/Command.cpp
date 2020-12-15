#include "defines.h"
#include "Command.h"
#include "structs.h"
#include <cstdio>
#include <cstring>



pString Command::Name() const //-V2506
{
    static const pString names[Count] =
    {
        "RequestData",
        "EnableChannel",
        "SetFormWave",
        "SetFrequency",
        "SetAmplitude",
        "SetOffset",
        "SetDuration",
        "SetDutyRatio",
        "SetPhase",
        "RunReset",
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
        "SetModeManipulation",
        "LoadFormDDS",
        "FreqMeasure",
        "Log",
        "FDrive_NumDirsAndFiles",
        "FDrive_Mount",
        "FDrive_RequestDir",
        "FDrive_RequestFile",
        "Test",
        "SetKoeffCalibration",
        "GetKoeffCalibration",
        "FDrive_RequestFileSize",
        "FDrive_RequestFileString",
        "FDrive_LoadFromExtStorage",
        "FDrive_GetPictureDDS",
        "SCPI_Data"
    };
    if(value < Count)
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

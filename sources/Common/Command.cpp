#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Command.h"
#include "structs.h"
#include "Utils/StringUtils.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pString Command::Name() const
{
    DEF_STRUCT(StructName, pString) names[Number] =
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
        "FDrive_LoadToFPGA",
        "FDrive_GetPictureDDS"
    };
    if(value < Number)
    {
        return names[value].val;
    }
    
    /// \todo Здесь страшный баг - при большинстве других значений SIZE процессор виснет, даже не заходя в функцию Command::Trace()
    /// Возможно, это происходит из-за использования CCM RAM
    const int SIZE = 23;
    static char buffer[SIZE + 1];
    snprintf(buffer, SIZE, "Неправильный параметр %d", value);

    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Command::Trace(const uint8 *buffer) const
{
    /// \todo Здесь страшный баг - при большинстве других значений SIZE процессор виснет, даже не заходя в функцию Command::Trace()
    static const int SIZE = 28;
    static char result[SIZE];
  
    snprintf(result, SIZE - 1, "%s %d ", Name(), buffer[1] + 1);
   
    switch(value)
    {
        case RequestData:
            break;
        case EnableChannel:
            std::strcat(result, buffer[2] ? "true" : "false");
            break;
        case SetFormWave:
            {
                Form form((Form::E)buffer[2]);
                std::strcat(result, form.Name(Language::RU));
            }
            break;
        case SetFrequency:
            std::strcat(result, Buffer2FloatString(buffer + 2));
            break;
        case SetAmplitude:
            std::strcat(result, Buffer2FloatString(buffer + 2));
            break;
        case SetOffset:
            std::strcat(result, Buffer2FloatString(buffer + 2));
            break;
        case SetDuration:
            break;
        case SetDutyRatio:
            break;
        case SetPhase:
            std::strcat(result, Buffer2FloatString(buffer + 2));
            break;
        case RunReset:
            break;
        case ModeDebug:
            break;
        case SetDelay:
            break;
        case WriteRegister:
            break;
        case SetDurationFall:
            break;
        case SetDurationRise:
            break;
        case SetDurationStady:
            break;
        case SetDutyFactor:
            break;
        case SetManipulation:
            std::strcat(result, buffer[2] == 0 ? "false" : "true");
            break;
        case SetManipulationDuration:
            std::strcat(result, Buffer2FloatString(buffer + 2));
            break;
        case SetManipulationPeriod:
            std::strcat(result, Buffer2FloatString(buffer + 2));
            break;
        case SetPacketPeriod:
            std::strcat(result, Buffer2FloatString(buffer + 2));
            break;
        case SetPacketNumber:
            break;
        case SetStartMode:
            break;
        case SetPeriod:
            break;
        case SetPolarity:
            break;
        case SetManipulationMode:
            std::strcat(result, buffer[2] ? "1" : "0");
            break;
        case LoadFormDDS:
            std::strcat(result, "точки");
            break;
        case FreqMeasure:
            break;
        case Log:
            break;
        case FDrive_NumDirsAndFiles:
            break;
        case FDrive_Mount:
            break;
        case FDrive_RequestDir:
            break;
        case FDrive_RequestFile:
            break;
        case Test:
            break;
        case SetKoeffCalibration:
            break;
        case GetKoeffCalibration:
            break;
        case FDrive_RequestFileSize:
            break;
        case FDrive_RequestFileString:
            break;
        case FDrive_LoadToFPGA:
            break;
        case FDrive_GetPictureDDS:
            break;
        case Number:
            break;
    }

    return result;
}

#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Command.h"
#include "structs.h"
#include "Utils/StringUtils.h"
#include "Log.h"
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pString Command::Name() const
{
    static const struct StructName
    {
        pString name;
        StructName(pString n) : name(n) {};
    }
    names[Number] =
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
        "FDrive_RequestFile"
    };
    if(value < Number)
    {
        return names[value].name;
    }
    
    /// \todo Здесь страшный баг - при большинстве других значений SIZE процессор виснет, даже не заходя в функцию Command::Trace()
    /// Возможно, это происходит из-за использования CCM RAM
    const int SIZE = 23;
    static char buffer[SIZE + 1];
    snprintf(buffer, SIZE, "Неправильный параметр %d", value);

    return buffer;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
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
            strlcat(result, buffer[2] ? "true" : "false", SIZE - 1);
            break;
        case SetFormWave:
            {
                Form form((Form::E)buffer[2]);
                strlcat(result, form.Name(Language::RU), SIZE - 1);
            }
            break;
        case SetFrequency:
            strlcat(result, Buffer2FloatString(buffer + 2), SIZE - 1);
            break;
        case SetAmplitude:
            strlcat(result, Buffer2FloatString(buffer + 2), SIZE - 1);
            break;
        case SetOffset:
            strlcat(result, Buffer2FloatString(buffer + 2), SIZE - 1);
            break;
        case SetDuration:
            break;
        case SetDutyRatio:
            break;
        case SetPhase:
            strlcat(result, Buffer2FloatString(buffer + 2), SIZE - 1);
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
            strlcat(result, buffer[2] == 0 ? "false" : "true", SIZE - 1);
            break;
        case SetManipulationDuration:
            strlcat(result, Buffer2FloatString(buffer + 2), SIZE - 1);
            break;
        case SetManipulationPeriod:
            strlcat(result, Buffer2FloatString(buffer + 2), SIZE - 1);
            break;
        case SetPacketPeriod:
            strlcat(result, Buffer2FloatString(buffer + 2), SIZE - 1);
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
            strlcat(result, buffer[2] ? "1" : "0", SIZE - 1);
            break;
        case LoadFormDDS:
            strlcat(result, "точки", SIZE - 1);
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
        case Number:
            break;
    }

    return result;
}

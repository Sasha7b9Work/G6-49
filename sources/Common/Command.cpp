#include "defines.h"
#include "Command.h"
#include "structs.h"
#include "Utils/StringUtils.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pString CommandPanel::Name() const
{
    static const struct StructName
    {
        pString name;
        StructName(pString n) : name(n) {};
    } names[Number] =
    {
        "None",
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
        "SetModeManipulation"
    };
    return names[value].name;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString CommandPanel::Trace(uint8 *buffer) const
{
    static char result[50];
    sprintf(result, "%s %d ", Name(), buffer[1] + 1);

    switch(value)
    {
        case None:
            break;
        case EnableChannel:
            strcat(result, buffer[2] ? "true" : "false");
            break;
        case SetFormWave:
            {
                Form form((Form::E)buffer[2]);
                strcat(result, form.Name(Language::RU));
            }
            break;
        case SetFrequency:
            strcat(result, Float2String(BitSet32(buffer + 2).floatValue));
            break;
        case SetAmplitude:
            break;
        case SetOffset:
            break;
        case SetDuration:
            break;
        case SetDutyRatio:
            break;
        case SetPhase:
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
            break;
        case SetManipulationDuration:
            break;
        case SetManipulationPeriod:
            break;
        case SetPacketPeriod:
            break;
        case SetPacketNumber:
            break;
        case SetStartMode:
            break;
        case SetPeriod:
            break;
        case SetPolarity:
            break;
        case SetModeManipulation:
            break;
        case Number:
            break;
    }

    return result;
}

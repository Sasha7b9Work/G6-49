#include "Command.h"


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

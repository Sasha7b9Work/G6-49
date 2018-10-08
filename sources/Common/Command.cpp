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

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString CommandPanel::Trace(uint8 *buffer) const
{
    static char result[50];
    sprintf(result, "%s ", Name());

    switch(value)
    {
        case None:
            break;
        case EnableChannel:
            strcat(result, buffer[2] ? "true" : "false");
            break;
        case SetFormWave:
            Form form((Form::E)buffer[2]);
            strcat(result, form.Name(Language::RU));
            break;
    }

    return result;
}

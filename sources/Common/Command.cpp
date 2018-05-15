#include "Command.h"
#include "defines.h"
#include "Log.h"
#include <Settings/Settings.h>
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static pString nameWaveForm[NumForms][2] =
{
    { "ÑÈÍÓÑ",    "SINE" },
    { "ÏÈËÀ",     "SAW" },
    { "ÈÌÏÓËÜÑ",  "IMPULSE" }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *Command_Name(CommandWrite command)
{
    static const char *names[NUM_COMMAND_WRITE] =
    {
        "SERVICE",
        "ENABLE",
        "SET FORM",
        "SET FREQUENCY",
        "SET AMPLITUDE",
        "SET OFFSET",
        "SET DURATION",
        "SET DUTYRATIO",
        "SET PHASE",
        "RUN RESET",
        "MODE DEBUG"
    };

    if (command >= NUM_COMMAND_WRITE)
    {
        static char buffer[10] = {0};
        sprintf(buffer, "%x", static_cast<uint8>(command));
        LOG_WRITE("ÎØÈÁÊÀ - ïðèíÿòî %s", buffer);
    }
    return const_cast<char *>(names[command]);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char * WaveForm::Name()
{
    return nameWaveForm[type][LANG];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 WaveForm::ToValue()
{
    return (uint8)type;
}

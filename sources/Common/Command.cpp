#include "Command.h"
#include "defines.h"
#include "Log.h"
#include <Settings/Settings.h>
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char * namesWaveForm[NumForms][2] =
{
    { "ÑÈÍÓÑ",        "SINE" },
    { "ÊÎÑÈÍÓÑ",      "COSINE" },
    { "ÌÅÀÍÄĞ",       "MEANDER"},
    { "ÏÈËÀ+",        "RAMP+" },
    { "ÏÈËÀ-",        "RAMP-"},
    { "ÒĞÅÓÃÎËÜÍÈÊ",  "TRIANGLE"},
    { "ÒĞÀÏÅÖÈß",     "TRAPEZE"},
    { "ÈÌÏÓËÜÑ",      "IMPULSE" },
    { "İÊÑÏÎÍÅÍÒÀ+",  "EXPONENT+"},
    { "İÊÑÏÎÍÅÍÒÀ-",  "EXPONENT-"},
    { "ØÓÌ",          "NOISE"},
    { "ÏĞÎÈÇÂÎËÜÍÛÉ", "FREE"}
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
        LOG_WRITE("ÎØÈÁÊÀ - ïğèíÿòî %s", buffer);
    }
    return const_cast<char *>(names[command]);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString WaveForm::Name() const
{
    return NAME_FORM(type, LANG);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint8 WaveForm::ToValue() const
{
    return (uint8)type;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
WaveForm::WaveForm(uint8 num) : type((Type_WaveForm)num)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString Register::Name() const
{
    static pString names[NumRegisters] =
    {
        "Ìóëüòèïëåêñîğ 1",
        "Ìóëüòèïëåêñîğ 2",
        "9952 Ñìåùåíèå 1",
        "9952 Ñìåùåíèå 2",
        "9952 ×àñò óğîâåíü",
        "9952 ×àñò ãèñòåğåçèñ"
    };

    return names[name];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Register::Register(uint8 n) : name((Name_Register)n)
{
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Register::Register(Name_Register n) : name(n)
{
}

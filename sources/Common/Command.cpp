#include "Command.h"
#include "defines.h"
#include "Log.h"
#include <Settings/Settings.h>
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const char *nameWaveForm[NumForms][2] =
{
    { "ÑÈÍÓÑ",    "SINE" },
    { "ÏÈËÀ",     "SAW" },
    { "ÈÌÏÓËÜÑ",  "IMPULSE" }
};

static const char *nameParameter[NumParameters][2] =
{
    { "×ÀÑÒÎÒÀ",      "FREQUENCY" },
    { "ÏÅÐÈÎÄ",       "PERIOD" },
    { "ÀÌÏËÈÒÓÄÀ",    "AMPLITUDE" },
    { "ÑÌÅÙÅÍÈÅ",     "OFFSET" },
    { "ÄËÈÒÅËÜÍÎÑÒÜ", "DURATION" },
    { "ÑÊÂÀÆÍÎÑÒÜ",   "DUTY RATIO" },
    { "ÔÀÇÀ",         "PHASE" },
    { "ÇÀÄÅÐÆÊÀ",     "DELAY" }
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
const char* WaveForm_Name(WaveForm form)
{
    return (char*)nameWaveForm[form][LANG];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char* Parameter_Name(WaveParameter parameter)
{
    return (char*)nameParameter[parameter][LANG];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char* Choice_Name(void* choice, int num)
{
    TypeItem type = ((Control *)choice)->Type();

    const char* retValue = 0;

    if (type == Item_Choice)
    {
        retValue = ((Choice *)choice)->names[num * 2 + LANGUAGE];
    }
    else if (type == Item_ChoiceParameter)
    {
        ChoiceParameter *param = (ChoiceParameter*)choice;

        for (int i = 0; i < NumParameters; i++)
        {
            if ((param->allowParameters).allow[i] == false)
            {
                continue;
            }
            if (num == 0)
            {
                retValue = (char*)nameParameter[i][LANGUAGE];
            }
            --num;
        }
    }

    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *ChoiceWaveParameter_CurrentName(void *choice)
{
    int num = *((ChoiceParameter *)choice)->numParameter;

    return (const char*)(nameParameter[num][LANG]);
}

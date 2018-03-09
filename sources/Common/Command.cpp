#include "Command.h"
#include "defines.h"
#include "Log.h"
#include <Settings/Settings.h>
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const char *nameWaveForm[NumForms][2] =
{
    { "яхмся",    "SINE" },
    { "охкю",     "SAW" },
    { "хлоскэя",  "IMPULSE" }
};

static const char *nameParameter[NumParameters][2] =
{
    { "вюярнрю",      "FREQUENCY" },
    { "оепхнд",       "PERIOD" },
    { "юлокхрсдю",    "AMPLITUDE" },
    { "ялеыемхе",     "OFFSET" },
    { "дкхрекэмнярэ", "DURATION" },
    { "яйбюфмнярэ",   "DUTY RATIO" },
    { "тюгю",         "PHASE" },
    { "гюдепфйю",     "DELAY" }
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
        LOG_WRITE("ньхайю - ОПХМЪРН %s", buffer);
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
    TypeItem type = TypeOfItem(choice);

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
    return (char*)(nameParameter[*(((Choice *)choice)->cell)][LANG]);
}

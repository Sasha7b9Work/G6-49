#include "Command.h"
#include "defines.h"
#include "Log.h"
#include <Settings/Settings.h>
#include <stdio.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char * namesWaveForm[WaveForm::Number][2] =
{
    { "СИНУС",        "SINE" },
    { "КОСИНУС",      "COSINE" },
    { "МЕАНДР",       "MEANDER"},
    { "ПИЛА+",        "RAMP+" },
    { "ПИЛА-",        "RAMP-"},
    { "ТРЕУГОЛЬНИК",  "TRIANGLE"},
    { "ТРАПЕЦИЯ",     "TRAPEZE"},
    { "ИМПУЛЬС",      "IMPULSE" },
    { "ЭКСПОНЕНТА+",  "EXPONENT+"},
    { "ЭКСПОНЕНТА-",  "EXPONENT-"},
    { "ШУМ",          "NOISE"},
    { "ПРОИЗВОЛЬНЫЙ", "FREE"}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *Command_Name(CommandPanel command)
{
    static const char *names[CommandPanel::Number] =
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

    if (command >= CommandPanel::Number)
    {
        static char buffer[10] = {0};
        sprintf(buffer, "%x", static_cast<uint8>(command));
        LOG_WRITE("ОШИБКА - принято %s", buffer);
    }
    return const_cast<char *>(names[command]);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString WaveForm::Name() const
{
    return NAME_FORM(type, LANG);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString Register::Name() const
{
    static pString names[Register::Number] =
    {
        "Мультиплексор 1",
        "Мультиплексор 2",
        "5697 Смещение 1",
        "5697 Смещение 2",
        "5697 Част уровень",
        "5697 Част гистерезис",
        "RG0 Управление",
        "RG1 Частота",
        "RG2 Умножитель",
        "RG3 Прямоуг А",
        "RG4 Прямоуг B",
        "RG5 Период импульсов А",
        "RG6 Длит. импульсов А",
        "RG7 Период импульсов B",
        "RG8 Длит. импульсов B",
        "RG9 Парам. частотомера",
        "RG10 Смещение",
        "Мультиплексор 3"
    };

    return names[name];
}

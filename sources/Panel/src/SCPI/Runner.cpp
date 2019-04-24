#include "defines.h"
#include "SCPI/Runner.h"
#include "SCPI/Parser.h"
#include "Hardware/VCP.h"
#include <cstring>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct StructCommand
{
    char   *symbols;
    void   (*func)();
};

static bool ProcessStructs(const StructCommand *commands);
/// ¬озвращает длину самой длиной команды из массива
static uint FindMaxLength(const StructCommand *commands);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SCPI::Runner::Execute()
{
    static const StructCommand commands[] =
    {
        {"*IDN?",       Process::IDN},
        {"*RST",        Process::RST},
        {"CHANNEL",     Process::Channel},
        {"FREQUENCY",   Process::Frequency},
        {"FORM",        Process::Form},
        {"AMPLITUDE",   Process::Amplitude},
        {nullptr, nullptr}
    };

    return ProcessStructs(commands);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool ProcessStructs(const StructCommand *commands)
{
    uint maxLength = FindMaxLength(commands);

    String *word = SCPI::Parser::GetWord();

    if (std::strlen(word->CString()) > maxLength)
    {
        return true;
    }

    const StructCommand *command = commands;

    while (command)
    {
        if (std::strcmp(word->CString(), command->symbols) == 0)
        {
            command->func();
            return true;
        }

        command++;
    }


    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint FindMaxLength(const StructCommand *commands)
{
    const StructCommand *current = commands;

    uint result = 0;

    while (current)
    {
        if (std::strlen(current->symbols) > result)
        {
            result = std::strlen(current->symbols);
        }

        current++;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SCPI::Runner::Process::IDN()
{
    VCP::Send("Test ID");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SCPI::Runner::Process::RST()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SCPI::Runner::Process::Channel()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SCPI::Runner::Process::Frequency()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SCPI::Runner::Process::Form()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SCPI::Runner::Process::Amplitude()
{

}

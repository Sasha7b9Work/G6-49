#include "defines.h"
#include "Display/Console.h"
#include "SCPI/Runner.h"
#include "SCPI/Parser.h"
#include "Hardware/VCP.h"
#include <cstring>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace SCPI
{
    namespace Runner
    {
        static bool ProcessStructs(const StructCommand *commands);
    }
}

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
        {"", nullptr}
    };

    return ProcessStructs(commands);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SCPI::Runner::ProcessStructs(const StructCommand *commands)
{
    if(Parser::words.Size() == 0U)
    {
        return false;
    }
    
    String *word = SCPI::Parser::GetWord();

    const StructCommand *command = commands;

    while (std::strlen(command->symbols) != 0)
    {
        if (std::strcmp(word->CString(), command->symbols) == 0)
        {
            Parser::RemoveWord(word);

            Result::E result = command->func();

            ProcessError(result);
    
            return true;
        }

        command++;
    }

    HandlerUnknownCommand();

    Parser::ClearList();

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SCPI::Runner::ProcessError(Result::E error)
{
    if (error == Result::InvalidSyntax)
    {
        HandlerInvalidSyntax();
    }
    else if (error == Result::UnknownCommand)
    {
        HandlerUnknownCommand();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SCPI::Runner::Result::E SCPI::Runner::Process::IDN()
{
    VCP::Send(Parser::GetWord()->CString());

    return Result::IsOk;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SCPI::Runner::Result::E SCPI::Runner::Process::RST()
{
    return Result::Count;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SCPI::Runner::Result::E SCPI::Runner::Process::Channel()
{
    return Result::Count;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SCPI::Runner::Result::E SCPI::Runner::Process::Frequency()
{
    return Result::Count;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SCPI::Runner::Result::E SCPI::Runner::Process::Form()
{
    return Result::Count;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
SCPI::Runner::Result::E SCPI::Runner::Process::Amplitude()
{
    return Result::Count;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SCPI::Runner::HandlerInvalidSyntax()
{
    VCP::Send("Invalid syntax");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SCPI::Runner::HandlerUnknownCommand()
{
    VCP::Send("Unknown command");
}


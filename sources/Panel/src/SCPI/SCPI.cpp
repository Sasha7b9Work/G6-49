#include "defines.h"
#include "SCPI.h"
#include "Command.h"
#include "Display/Console.h"
#include "SCPI/Commands.h"
#include <cstdlib>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIZE_BUFFER (1024)
static uint8 buffer[SIZE_BUFFER];

static uint pointer = 0;

/// Добавление нового байта данных
static void AddByte(uint8 byte);
/// Исполнить команду из буфера, если таковая имеется
static void RunBuffer();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SCPI::Handler::Processing(SimpleMessage *msg)
{
    msg->ResetPointer();

    ::Command::E command = (::Command::E)msg->TakeByte();

    if (command == ::Command::SCPI_Data)
    {
        uint length = msg->TakeWord();

        for (uint i = 0; i < length; i++)
        {
            AddByte(msg->TakeByte());

            RunBuffer();
        }
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void AddByte(uint8 byte)
{
    buffer[pointer++] = byte;

    if (pointer == SIZE_BUFFER)
    {
        pointer = SIZE_BUFFER - 1;

        std::memcpy(buffer, buffer + 1, SIZE_BUFFER - 1);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void RunBuffer()
{
    static const StructCommand commands[] =
    {
        {"*IDN?", SCPI::ProcessIDN},
        {nullptr, nullptr}
    };

    uint8 *addressLast = SCPI::ProcessingCommand(commands, buffer);

    if (addressLast)
    {

    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 *SCPI::ProcessingCommand(const StructCommand * /*commands*/, uint8 * /*data*/)
{
    return nullptr;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool SCPI::ProcessIDN(uint8 * /*data*/)
{
    return false;
}

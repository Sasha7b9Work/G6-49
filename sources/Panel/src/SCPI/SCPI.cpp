#include "defines.h"
#include "SCPI.h"
#include "Command.h"
#include "Display/Console.h"
#include <cstdlib>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//static void AddNewData(uint8 *data, uint size);
static void AddString(char *string);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool SCPI::Handler::Processing(Message *msg)
{
    msg->ResetPointer();

    Command::E command = (Command::E)msg->TakeByte();

    if (command == Command::SCPI_Data)
    {
        uint length = msg->TakeWord();

        char *data = (char *)std::malloc(length);

        if (data)
        {
            data[0] = '\0';

            for (; length > 0; length--)
            {
                char cat[2] = { (char)msg->TakeByte(), 0 };
                std::strcat(data, cat);
            }

            AddString(data);
        }

        std::free(data);
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void AddString(char *string)
{
    Console::AddString(string);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//static void AddNewData(uint8 *, uint)
//{
//}

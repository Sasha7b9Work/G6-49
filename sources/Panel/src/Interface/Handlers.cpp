#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Command.h"
#include "Message.h"
#include "Handlers.h"
#include "Display/Console.h"
#include "FrequencyMeter/FrequencyMeter.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Handlers::Processing(Message *msg)
{
    uint8 command = msg->TakeByte();

    if (command == Command::RequestData)
    {
    }
    else if (command == Command::FreqMeasure)
    {
        FrequencyMeter::SetMeasure(msg->TakeWord());
        return true;
    }
    else if (command == Command::Log)
    {
        Console::AddString((pString)(msg->Data() + 1));
        return true;
    }
    else
    {
        LOG_WRITE_FINALIZE("Поступила неизвестная команда %d", command);
    }

    return false;
}

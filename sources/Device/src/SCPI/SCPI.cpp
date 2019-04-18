#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "SCPI.h"
#include "Command.h"
#include "Message.h"
#include "Hardware/VCP.h"
#include "Interface/InterfaceDevice.h"
#include "Utils/Buffer.h"
#include "Utils/StringUtils.h"
#include <cstring>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SCPI::AddNewData(uint8 *buffer, uint length)
{
    uint size = length + 1 + 4;

    Message *message = new Message(size, Command::SCPI_Data);

    message->PutWord(length);

    std::memcpy(message->Data(1), buffer, length);

    Interface::AddMessageForTransmit(message);
}

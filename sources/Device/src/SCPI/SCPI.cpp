#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "SCPI.h"
#include "Command.h"
#include "Message.h"
#include "Hardware/VCP.h"
#include "Utils/Buffer.h"
#include "Utils/StringUtils.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void SCPI::AddNewData(uint8 *buffer, uint length)
{
    uint size = length + 1 + 4;

    SimpleMessage message(size, Command::SCPI_Data);

    message.PutWord(length);

    message.PutData(buffer, length);

    message.Transmit();
}

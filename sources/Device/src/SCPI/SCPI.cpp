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
    Message::SCPI::Data(buffer, length).Transmit();
}

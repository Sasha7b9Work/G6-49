#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Message.h"
#include "SCPI/VCP.h"
#include "Display/Console.h"
#include "Utils/Buffer.h"
#include <cstring>
#endif



void VCP::SendStringAsynch(const char *data)
{
    char *mes = const_cast<char *>(data);

    Message::SCPI::Data(reinterpret_cast<uint8 *>(mes), std::strlen(data)).Transmit();
}

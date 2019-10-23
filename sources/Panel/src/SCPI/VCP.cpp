#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Message.h"
#include "SCPI/VCP.h"
#include "Display/Console.h"
#include "Utils/Buffer.h"
#include <cstring>
#endif



void VCP::Send(const char *data)
{
    uint size = std::strlen(data);

    Buffer buffer(size + 2);
    buffer.Data()[0] = ':';
    std::memcpy(buffer.Data() + 1, data, size);
    buffer.Data()[size + 1] = 0x0d;

    Message::SCPI::Data(buffer.Data(), buffer.Size()).Transmit();
}

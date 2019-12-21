#include "defines.h"
#include "Message.h"
#include "Hardware/VCP_p.h"
#include "Display/Console.h"
#include "Utils/Buffer.h"
#include <cstring>


void VCP::SendStringAsynch(const char *data)
{
    char *mes = const_cast<char *>(data);

    Message::SCPI::Data(reinterpret_cast<uint8 *>(mes), std::strlen(data)).Transmit();
}

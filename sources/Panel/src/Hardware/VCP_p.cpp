#include "defines.h"
#include "common/Messages.h"
#include "Hardware/VCP_p.h"
#include "Display/Console.h"
#include "Utils/Buffer.h"
#include <cstring>


void PVCP::SendStringAsynch(const char *data)
{
    char *mes = const_cast<char *>(data); //-V2567

    Message::SCPI::Data(reinterpret_cast<uint8 *>(mes), static_cast<int>(std::strlen(data))).Transmit(); //-V2513
}

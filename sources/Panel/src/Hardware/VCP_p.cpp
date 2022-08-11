// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "common/Messages.h"
#include "Hardware/VCP_p.h"
#include "Display/Console.h"
#include "Utils/Buffer.h"
#include <cstring>


void PVCP::SendStringAsynch(const char *data)
{
    char *mes = const_cast<char *>(data);

    Message::SCPI::Data(reinterpret_cast<uint8 *>(mes), (int)std::strlen(data)).Transmit();
}

#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Command.h"
#include "log.h"    
#include "Transceiver.h"
#include "Hardware/Modules/SPI.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::Transmit(Message *message)
{
    SPI1_::Transmit(message->Size(), 10);

    SPI1_::Transmit(message->buffer, message->Size(), 
        (message->Size() > 100U) ? 200U : 10U);
}

#include "defines.h"
#include "Command.h"
#include "log.h"    
#include "Transceiver.h"
#include "Hardware/HAL/HAL.h"


void Transceiver::Transmit(SimpleMessage *message)
{
    SPI1_::Transmit(message->Size(), 10);

    SPI1_::Transmit(message->Data(), message->Size(), 
        (message->Size() > 100U) ? 200U : 10U);
}

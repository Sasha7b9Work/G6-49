#include "defines.h"
#include "common/Command.h"
#include "common/Transceiver.h"
#include "Hardware/HAL/HAL.h"


void Transceiver::Transmit(SimpleMessage *message)
{
    HAL_SPI1::Transmit(message->Size(), 10);

    HAL_SPI1::Transmit(message->Data(), message->Size(), 
        (message->Size() > 100U) ? 200U : 10U);
}

#pragma once
#include "Messages.h"


namespace Transceiver
{
    void Transmit(SimpleMessage *message);

    bool Receive(SimpleMessage *message);
};

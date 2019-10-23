#pragma once
#include "Message.h"



namespace Transceiver
{
    void Transmit(SimpleMessage *message);

    bool Receive(SimpleMessage *message);
};

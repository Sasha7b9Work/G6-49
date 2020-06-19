#pragma once
#include "Messages.h"


/*

    Передача должна производиться так.

*/


namespace Transceiver
{
    void Transmit(SimpleMessage *message);

    bool Receive(SimpleMessage *message);
};

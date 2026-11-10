// (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "Messages.h"


/*

    Передача должна производиться так.

*/


namespace Transceiver
{
    void Transmit(SimpleMessage *);

    bool Receive(SimpleMessage *);
};

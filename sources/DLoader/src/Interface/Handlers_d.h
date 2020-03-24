#pragma once
#include "common/Messages.h"


struct DHandlers
{
    static void Processing(SimpleMessage *message);

private:
    static void E(SimpleMessage *);

    static void SendData(SimpleMessage *);
};

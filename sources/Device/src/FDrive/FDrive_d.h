#pragma once
#include <ff.h>
#include "common/Messages.h"


struct DDrive
{
    static void Init();

    static void Update();

    static USBH_HandleTypeDef hUSB_Host;

    struct Handler
    {
        static void Processing(SimpleMessage *msg);
    };
};

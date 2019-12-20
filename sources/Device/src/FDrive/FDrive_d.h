#pragma once
#include <ff.h>
#include "Message.h"



namespace FDrive
{
    void Init();

    void Update();

    extern USBH_HandleTypeDef hUSB_Host;

    namespace Handler
    {
        void Processing(SimpleMessage *msg);
    };
};

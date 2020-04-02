#pragma once
#include <ff.h>
#include "common/Messages.h"


struct DLDrive
{
    static void Init();

    static void Update();

    static USBH_HandleTypeDef hUSB_Host;

    struct Handler
    {
        static void Processing(SimpleMessage *msg);
    private:
        static SimpleMessage *msg;
        static void GetNumDirsAndFiles();
        static void RequestFile();
        static void RequestFileSize();
    };
};

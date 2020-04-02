#pragma once
#include <ff.h>


struct StructForReadDir;


struct DLDrive
{
    static void Init();

    static void Update();

    static USBH_HandleTypeDef *handle;
};

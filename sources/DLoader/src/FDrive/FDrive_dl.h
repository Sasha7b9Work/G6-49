#pragma once
#include <ff.h>
#include "common/Messages.h"


struct DLDrive
{
    static void Init();

    static void Update();

    static USBH_HandleTypeDef *handle;

    struct Handler
    {
        static void GetNumDirsAndFiles(char *fullPath);
        static void RequestFile(int num, char *fullPath);
        static void RequestFileSize(int num, char *path);
    };
};

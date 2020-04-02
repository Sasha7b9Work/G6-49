#pragma once
#include <ff.h>


struct DLDrive
{
    static void Init();

    static void Update();

    static void GetNumDirsAndFiles(char *fullPath);

    static void RequestFile(int num, char *fullPath);

    static void RequestFileSize(int num, char *path);

    static USBH_HandleTypeDef *handle;
};

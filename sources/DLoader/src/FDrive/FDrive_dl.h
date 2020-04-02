#pragma once
#include <ff.h>


struct DLDrive
{
    static void Init();

    static void Update();

    // Получает количество каталогов и файлов в данной директории
    static void GetNumDirsAndFiles(const char *fullPath, int *numDirs, int *numFiles);

    static void RequestFile(int num, char *fullPath);

    static void RequestFileSize(int num, char *path);

    static int GetFileSize(const char *fullPath);

    static USBH_HandleTypeDef *handle;
};

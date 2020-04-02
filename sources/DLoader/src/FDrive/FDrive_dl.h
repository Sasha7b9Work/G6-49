#pragma once
#include <ff.h>


struct StructForReadDir;


struct DLDrive
{
    static void Init();

    static void Update();

    // Получает количество каталогов и файлов в данной директории
    static void GetNumDirsAndFiles(const char *fullPath, int *numDirs, int *numFiles);

    static void RequestFile(int num, char *fullPath);

    static void RequestFileSize(int num, char *path);

    static int GetFileSize(const char *fullPath);

    // Получить имя numFile-го файла из каталога fullPath
    static bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut);
    
    // Читает значения отсчётов сигнала из файла name
    static bool ReadFloats(float values[4096], char *name);
    
    // Получить имя numDir-го каталога из каталога fullPath
    static bool GetNameDir(const char *fullPath, int numDir, char *nameDirOut, StructForReadDir *s);

    static USBH_HandleTypeDef *handle;
};

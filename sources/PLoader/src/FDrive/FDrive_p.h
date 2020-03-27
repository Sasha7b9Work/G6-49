#pragma once
#include "common/Messages.h"



struct FDrive
{
    /// Начальная инициализация
    static void Init();

    /// Что показываем - каталоги или файлы
    enum View
    {
        Dirs,
        Files
    };
    
    static View view;

    static char *CurrentDirectory();

    struct Handler
    {
        static bool Processing(SimpleMessage *msg);
    private:
        static SimpleMessage *msg;
        static bool E();
        static bool IsMount();
    };

private:
    enum Mount
    {
        Disconnect,
        Mounted,
        Failed
    };
    /// Текущее состояние флешки
    static Mount mounted;
    /// Путь к текущему каталогу
    static char directory[255];
    /// Если true - идёт загрузка сигнала с флешки в память
    static bool inStateWaitCompleteLoad;
};

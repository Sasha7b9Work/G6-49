#pragma once
#include "Message.h"



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

    /// Отрисовка файло-менеджера
    static void Draw();
    /// Обработка нажатия кнопки "Вверх"
    static void PressUp();
    /// Обработка нажатия кнопки "Вниз"
    static void PressDown();
    /// Обработка нажатия кнопки "Выбрать"
    static void PressChoose();

    static char *CurrentDirectory();

    struct Handler
    {
        static bool Processing(SimpleMessage *msg);
    private:
        static SimpleMessage *msg;
        static bool E();
        static bool IsMount();
        static bool GetNumDirsAndFiles();
        static bool RequestFile();
        static bool RequestFileSize();
        static bool LoadFromExtStorage();
    };
};

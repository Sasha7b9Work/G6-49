// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Messages.h"


namespace FDrive
{
    // Начальная инициализация
    void Init();

    // Что показываем - каталоги или файлы
    enum View
    {
        Dirs,
        Files
    };
    
    extern View view;

    // Отрисовка файло-менеджера
    void Draw();
    
    // Обработка нажатия кнопки "Вверх"
    void PressUp();
    
    // Обработка нажатия кнопки "Вниз"
    void PressDown();
    
    // Обработка нажатия кнопки "Выбрать"
    void PressChoose();

    char *CurrentDirectory();

    void SaveScreenToFlash();

    struct Handler
    {
        static bool Processing(SimpleMessage *msg);
    private:
        static SimpleMessage *msg;
        static bool IsMount();
        static bool GetNumDirsAndFiles();
        static bool RequestFile();
        static bool RequestFileSize();
        static bool LoadDDSfromFile();
    };
};

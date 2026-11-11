// (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
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

    bool IsMounted();

    bool ExistFile(pchar);

    namespace Handler
    {
        bool Processing(SimpleMessage *msg);
    };
};

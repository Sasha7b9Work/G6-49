#pragma once
#include "Message.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FDrive
{
    /// Начальная инициализация
    void Init();

    /// Что показываем - каталоги или файлы
    enum View
    {
        Dirs,
        Files
    };
    
    extern View view;

    /// Отрисовка файло-менеджера
    void Draw();
    /// Обработка нажатия кнопки "Вверх"
    void PressUp();
    /// Обработка нажатия кнопки "Вниз"
    void PressDown();
    /// Обработка нажатия кнопки "Выбрать"
    void PressChoose();

    char *CurrentDirectory();

    namespace Handler
    {
        bool Processing(SimpleMessage *msg);
    };
};

#pragma once
#include "Message.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FDrive
{
public:
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
    /// Написать список каталогов
    static void DrawDirs();
    /// Написать список файлов
    static void DrawFiles();

    class Handler
    {
    friend class Handlers;
        static bool Processing(Message *msg);
    };

    /// Путь к текущему каталогу
    static char directory[255];

private:

    friend class Graphics;
};

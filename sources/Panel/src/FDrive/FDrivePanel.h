#pragma once
#include "Message.h"


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

private:

    friend class Graphics;

    /// Возвращает количество каталогов и файлов в каталоге directory
    static bool GetNumDirsAndFiles(pString directory, uint *numDirs, uint *numFiles);

    static void RequestNameDir(uint numDir, pString directory);

    static void RequestNameFile(uint numFile, pString directory);
};

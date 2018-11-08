#pragma once


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

private:
    friend class Graphics;
    friend class Interface;

    /// Возвращает количество каталогов и файлов в каталоге directory
    static bool GetNumDirsAndFiles(pString directory, uint *numDirs, uint *numFiles);

    static void RequestNameDir(uint numDir, pString directory);

    static void RequestNameFile(uint numFile, pString directory);

    static void HandlerInterface(uint8 *data);
};

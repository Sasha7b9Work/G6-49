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

    class Graphics
    {
    friend class FDrive;

    public:
        /// Отрисовка файло-менеджера
        static void Draw();

    private:

        static void Init();
    };

private:
    friend class Graphics;
    friend class Interface;

    /// Посылает запрос на количество файлов и каталогов в каталоге directory
    static void RequestNumDirsAndFiles(pString directory);

    static void RequestNameDir(uint numDir, pString directory);

    static void RequestNameFile(uint numFile, pString directory);

    static void HandlerInterface(uint8 *data);

    /// Написать список каталогов
    static void DrawDirs();
    /// Написать список файлов
    static void DrawFiles();
};

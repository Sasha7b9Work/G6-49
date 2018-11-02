#pragma once


class FDrive
{
public:
    /// Начальная инициализация
    static void Init();
    /// Возвращает количество файлов и каталогов в каталоге directory
    static void GetNumDirsAndFiles(pString directory, int *numDirs, int *numFiles);

    class Graphics
    {
    friend class FDrive;

    public:
        /// Отрисовка файло-менеджера
        static void Draw();

    private:

        static void Init();
    };
};

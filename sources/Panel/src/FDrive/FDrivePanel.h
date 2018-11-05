#pragma once


class FDrive
{
friend class Interface;

public:
    /// Начальная инициализация
    static void Init();
    /// true, если флешка подключена
    static bool IsConnected();

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
    /// Посылает запрос на количество файлов и каталогов в каталоге directory
    static void RequestNumDirsAndFiles(pString directory);
    /// Устанавливает признак присоединённой флешки
    static void SetConnected(bool connected);

    static void HandlerSetNumDirsAndFiles(uint numDirs, uint numFiles);
};

#pragma once


class Hint
{
public:
    /// Возвращает true, если нужно выводить подсказку на экран
    static bool Show();
    /// Переключить вывод подсказки - если выводилась - не выводить, и наоборот
    static void Switch();

private:
    static bool show;
};

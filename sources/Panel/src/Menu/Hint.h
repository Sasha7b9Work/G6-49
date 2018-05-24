#pragma once
#include "Menu/MenuItems.h"


class Hint
{
public:
    /// Возвращает true, если нужно выводить подсказку на экран
    static bool Show();
    /// Переключить вывод подсказки - если выводилась - не выводить, и наоборот
    static void Switch();
    /// Отрисовка подсказки
    static void Draw();

    static void ClearItem();

    static void SetItem(const Control * const item);

private:
    /// Если true - нужно выводить подсказку на экран
    static bool show;
    /// Контрол, для которого нужно выводить подсказку
    static const Control *item;
};

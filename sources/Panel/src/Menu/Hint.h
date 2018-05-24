#pragma once
#include "Menu/MenuItems.h"
#include "Hardware/Controls.h"


class Hint
{
public:
    /// Возвращает true, если нужно выводить подсказку на экран
    static bool Show();
    /// Переключить вывод подсказки - если выводилась - не выводить, и наоборот
    static void Switch();
    /// Отрисовка подсказки
    static void Draw();
    /// Назначение контрола, для которого нужно выводить подсказку
    static void SetItem(const Item * const item);
    /// Обработать поворот ручки (листание страниц). Возвращает false, еcли поворот ручки не пригодился
    static void ProcessGovernor(Control gov);

private:
    /// Если true - нужно выводить подсказку на экран
    static bool show;
    /// Контрол, для которого нужно выводить подсказку
    static const Item *item;

    static Control control;
};

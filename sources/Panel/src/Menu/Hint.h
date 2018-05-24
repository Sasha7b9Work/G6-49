#pragma once
#include "Menu/MenuItems.h"
#include "Hardware/Controls.h"


class Hint
{
public:
    /// Отрисовка подсказки
    static void Draw();
    /// Орган упралвения. Возвращает false, если обработка не произошла
    static bool ProcessControl(StructControl strCtrl);

private:
    /// Если true - нужно выводить подсказку на экран
    static bool show;
    /// Контрол, для которого нужно выводить подсказку
    static const Item *item;

    static Control control;
    /// Количество страниц в подсказке
    static int numPages;
};

// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"


namespace Hint
{
    // Отрисовка подсказки
    void Draw();

    // Орган упралвения. Возвращает false, если обработка не произошла
    bool ProcessControl(const Control &);
};

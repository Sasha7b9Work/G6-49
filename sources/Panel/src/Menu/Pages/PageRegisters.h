#pragma once
#include "Menu/MenuItems.h"


class PageRegisters
{
public:
    static void Draw();

    static Page *pointer;

    static Name_Register currentRegister;

private:
    /// Написать регистры
    static void DrawRegisters(int x, int y);

    static void DrawInputWindow();
};

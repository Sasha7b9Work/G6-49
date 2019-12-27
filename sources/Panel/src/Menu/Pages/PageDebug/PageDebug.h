#pragma once
#include "Menu/MenuItems.h"


class PageDebug
{
public:
    static Page *self;

    class PageRegisters
    {
    public:

        static void Draw();

    private:
        /// Написать регистры
        static void DrawRegisters(int x, int y);

        static void DrawInputWindow();
    };

    class SubRange
    {
    public:
        static Page *self;
    };
};

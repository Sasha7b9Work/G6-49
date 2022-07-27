// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"


struct Color
{
    Color(uint8 v);
    static Color WHITE;
    static Color BLACK;
    static Color RED;
    static Color GREEN;
    static Color BLUE;
    static Color BLUE_50;
    static Color GREEN_1F;
    static Color GREEN_3F;
    static Color GRAY_50;
    static Color GRAY_1F;
    static Color GRAY_2F;
    static Color GRAY_3F;
    static Color GRAY_4F;
    static Color GRAY_5F;

    static Color NUMBER;

    unsigned char value;
};

bool operator!=(const Color &left, const Color &right);


extern const unsigned int colors[255];
template<typename T>
constexpr auto COLOR(T color) { return (colors[color]); }

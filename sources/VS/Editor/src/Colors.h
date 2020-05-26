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
    static Color DARK_GREEN_1F;
    static Color DARK_GREEN_3F;
    static Color GRAY_50;

    static Color NUMBER;

    unsigned char value;
};

bool operator!=(const Color &left, const Color &right);


extern const unsigned int colors[255];
template<typename T>
constexpr auto COLOR(T color) { return (colors[color]); }

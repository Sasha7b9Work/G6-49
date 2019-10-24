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

    static Color NUMBER;

    uint8 value;
};

bool operator!=(const Color &left, const Color &right);


extern const uint colors[255];
#define COLOR(color) (colors[color])
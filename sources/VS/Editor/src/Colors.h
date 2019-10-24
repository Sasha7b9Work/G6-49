#pragma once
#include "defines.h"


struct Color
{
    Color(uint8 v);
    static Color WHITE;
    static Color BLACK;

    static Color NUMBER;

    uint8 value;
};

bool operator!=(const Color &left, const Color &right);


extern uint colors[255];
#define COLOR(color) (colors[color])
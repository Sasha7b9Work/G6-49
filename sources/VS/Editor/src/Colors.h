#pragma once


struct Color
{
    Color(unsigned char v);
    static Color WHITE;
    static Color BLACK;
    static Color RED;
    static Color GREEN;
    static Color BLUE;

    static Color NUMBER;

    unsigned char value;
};

bool operator!=(const Color &left, const Color &right);


extern const unsigned int colors[255];
template<typename T>
constexpr auto COLOR(T color) { return (colors[color]); }

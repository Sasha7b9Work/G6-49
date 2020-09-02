#include "Colors.h"
#include "defines.h"


Color Color::WHITE(0);
Color Color::BLACK(1);
Color Color::RED(2);
Color Color::GREEN(3);
Color Color::BLUE(4);
Color Color::DARK_GREEN_1F(5);
Color Color::DARK_GREEN_3F(6);
Color Color::GRAY_50(7);

Color Color::NUMBER(255);


const uint colors[255]
{
    0x00ffffff,     // WHITE
    0x00000000,     // BLACK
    0x00ff0000,     // RED
    0x0000ff00,     // GREEN
    0x000000ff,     // BLUE
    0x00001f00,     // DARK_GREEN_1F
    0x00003f00,     // DARK_GREEN_2F
    0x00808080      // GRAY_50
};


Color::Color(uint8 v) : value(v) {}


bool operator!=(const Color &left, const Color &right)
{
    return left.value != right.value;
}

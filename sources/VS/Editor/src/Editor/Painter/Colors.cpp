// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "Colors.h"
#include "defines.h"


Color Color::WHITE(0);
Color Color::BLACK(1);
Color Color::RED(2);
Color Color::GREEN(3);
Color Color::BLUE(4);
Color Color::BLUE_50(5);
Color Color::GREEN_1F(6);
Color Color::GREEN_3F(7);
Color Color::GRAY_50(8);
Color Color::GRAY_1F(9);
Color Color::GRAY_2F(10);
Color Color::GRAY_3F(11);
Color Color::GRAY_4F(12);
Color Color::GRAY_5F(13);

Color Color::NUMBER(255);


const uint colors[255]
{
    0x00ffffff,     // WHITE
    0x00000000,     // BLACK
    0x00ff0000,     // RED
    0x0000ff00,     // GREEN
    0x000000ff,     // BLUE
    0x0000007f,     // BLUE_50
    0x00001f00,     // GREEN_1F
    0x00003f00,     // GREEN_3F
    0x00808080,     // GRAY_50
    0x001F1F1F,     // GRAY_1F
    0x002F2F2F,     // GRAY_2F
    0x003F3F3F,     // GRAY_3F
    0x004F4F4F,     // GRAY_4F
    0x005F5F5F      // GRAY_5F
};


Color::Color(uint8 v) : value(v) {}


bool operator!=(const Color &left, const Color &right)
{
    return left.value != right.value;
}

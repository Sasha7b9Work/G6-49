#include "Colors.h"


Color Color::WHITE(0);
Color Color::BLACK(1);

Color Color::NUMBER(255);


uint colors[255]
{
    0x00000000,
    0xffffffff
};


Color::Color(uint8 v) : value(v)
{

}


bool operator!=(const Color &left, const Color &right)
{
    return left.value == right.value;
}

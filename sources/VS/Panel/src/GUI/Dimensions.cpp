// 2022/8/1 10:04:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "GUI/Dimensions.h"


// ≈сли это определено - увеличенный размер
//#define BIG_SIZE


namespace Size
{
#ifdef BIG_SIZE
    static const int d = 2;
#else
    static const int d = 1;
#endif
}


bool Size::IsBig()
{
#ifdef BIG_SIZE
    return true;
#else
    return false;
#endif
}

int Size::Client::Width()
{
#ifdef BIG_SIZE
    return 1000;
#else
    return 680;
#endif
}


int Size::Client::Height()
{
    return 240 * d;
}


int Size::Screen::Width()
{
    return 320 * d;
}


int Size::Screen::Height()
{
    return 240 * d;
}


int Size::Buttons::X0()
{
    return Screen::Width() + 20;
}

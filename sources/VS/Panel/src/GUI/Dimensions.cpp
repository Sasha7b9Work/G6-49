// 2022/8/1 10:04:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "GUI/Dimensions.h"


// ≈сли это определено - увеличенный размер
#define BIG_SIZE


namespace Size
{
#ifdef BIG_SIZE
    static const int d = 2;
#else
    static const int d = 1;
#endif
}



int Size::Frame::Client::Width()
{
    return 680 * d;
}


int Size::Frame::Client::Height()
{
    return 240 * d;
}

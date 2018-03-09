#pragma once


#define HINT_MODE_ENABLED false
#define SHOW_DEBUG_MENU   true
#define NEED_FINISH_DRAW  (gBF.needFinishDraw)


struct BitField
{
    uint needFinishDraw;    ///< ≈сли 1, то нужно немедленно завершить отрисовку и вывести на экран то, что уже нарисовано.
};

extern volatile BitField gBF;

struct PackedTime
{
    uint hours : 5;
    uint minutes : 6;
    uint seconds : 6;
    uint year : 7;
    uint month : 4;
    uint notUsed0 : 4;
    uint day : 5;
    uint notUsed1 : 27;
};

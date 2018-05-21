#pragma once
#include "defines.h"


#define IN_MODE_SHOW_HINTS (gBF.showHints)


struct BitField
{
    uint showHints : 1;     ///< Установленное в 1 значение означает, что прибор находится режиме показа подсказок
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

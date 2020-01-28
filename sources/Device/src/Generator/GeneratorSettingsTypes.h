#pragma once
#include "common/CommonTypes.h"


struct TypeWave
{
    enum E
    {
        Sin,
        Cos
    } value;
    TypeWave(E v) : value(v) {};
};

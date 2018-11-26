#pragma once
#include "defines.h"


struct ParamValue
{
    uint64 value;       // Значение параметра в единицах измерения "нано". Установленный в 

    explicit ParamValue(uint64 v) : value(v) {}

    float ToFloat() const;

    int Sign() const;

    uint64 Abs() const;

    void Divide(uint div);

    void Multiplie(uint mul);

    void SetSign(int sign);
};

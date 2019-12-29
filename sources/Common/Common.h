#pragma once
#include "defines.h"


struct FloatValue
{
    explicit FloatValue(uint64 v) : value(v) {}

    explicit FloatValue(float v);

    float ToFloat() const;

    void FromFloat(float v);

    void FromUINT64(uint64 v) { value = v; }

    uint64 ToUINT64() const { return value; }

    int Sign() const;

    uint64 Abs() const;

    void Divide(uint div);

    void Multiplie(uint mul);
    /// ”величить на заданное значение
    void Add(float v);

    void SetSign(int sign);
    /// ¬озвращает целую часть числа
    int Integer() const;
    /// ¬озвращает дробную часть числа. numDigits - требуемое количество знакомест после зап€той
    int Fract(int numDigits) const;

private:

    uint64 value;       // «начение параметра в единицах измерени€ "нано". ”становленный в "1" старший бит означает, что число отрицательное
};

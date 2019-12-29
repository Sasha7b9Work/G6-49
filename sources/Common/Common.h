#pragma once
#include "defines.h"
#include "structs.h"


struct FloatValue
{
    explicit FloatValue(uint64 v)
    {
        FromUINT64(v);
    }

    explicit FloatValue(float v);

    float ToFloat() const;

    void FromFloat(float v);

    void FromUINT64(uint64 v)
    {
        _value.dword = v;
    }

    uint64 ToUINT64() const
    {
        return _value.dword;
    }

    int Sign() const;

    uint64 Abs() const;

    void Divide(uint div);

    void Multiplie(uint mul);
    /// Увеличить на заданное значение
    void Add(float v);

    void SetSign(int sign);

    uint Integer() const
    {
        return _value.word1;
    }

    uint Fract() const
    {
        return _value.word0;
    }

private:
    /// В младшей половине хранится дробная часть, в старшей - целая. Старший бит целой части отвечает за знак.
    BitSet64 _value;
};

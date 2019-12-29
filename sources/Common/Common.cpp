#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)   // unreferenced inline function has been removed
#endif

#include "Common.h"
#include <cmath>

#ifdef WIN32
#pragma warning(pop)
#endif



FloatValue::FloatValue(float v)
{
    FromFloat(v);
}


void FloatValue::FromFloat(float v)
{
    _value.dword = static_cast<uint64>(std::fabsf(v) * 1.E9F);

    if (v < 0.0F)
    {
        SetSign(-1);
    }
}


float FloatValue::ToFloat() const
{
    return Abs() / 1E9F * static_cast<float>(Sign());
}


int FloatValue::Sign() const
{
    //                    76543210
    return (Integer() & 0x80000000U) ? -1 : 1;
}


uint64 FloatValue::Abs() const
{
    //    fedcba9876543210
    return (_value.dword & 0x7fffffffffffffff);
}


void FloatValue::Divide(uint div)
{
    int sign = Sign();

    SetSign(1);

    _value.dword /= div;

    if(sign < 0)
    {
        SetSign(-1);
    }
}


void FloatValue::Multiplie(uint mul)
{
    int sign = Sign();

    SetSign(1);

    _value.dword *= mul;

    if(sign < 0)
    {
        SetSign(-1);
    }
}


void FloatValue::SetSign(int sign)
{
    if (sign > 0)
    {
        _value.word1 &= 0x7fffffff;     // ќбнул€ем признак отрицательного числа
    }
    else
    {
        //                76543210
        _value.word1 |= 0x80000000U;    // ”станавливаем признак отрицательного числа
    }
}


void FloatValue::Add(float v)
{
    FromFloat(ToFloat() + v);
}

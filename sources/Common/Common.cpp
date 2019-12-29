#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)   // unreferenced inline function has been removed
#endif

#include "Common.h"
#include "Utils/Math.h"
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
    int sign = (v < 0.0F) ? -1 : 1;

    value = static_cast<uint64>(std::fabsf(v) * 1.E9F);

    if (sign < 0)
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
    //    fedcba9876543210
    return (value & 0x8000000000000000U) ? -1 : 1;
}


uint64 FloatValue::Abs() const
{               //    fedcba9876543210
    return (value & 0x7fffffffffffffff);
}


void FloatValue::Divide(uint div)
{
    value /= div;
}


void FloatValue::Multiplie(uint mul)
{
    value *= mul;
}


void FloatValue::SetSign(int sign)
{
    if (sign > 0)
    {
        value &= 0x8000000000000000U;       /// \todo как это может работать?
    }
    else
    {
        //         fedcba9876543210
        value |= 0x8000000000000000U;           // Устанавливаем признак отрицательного числа
    }
}


void FloatValue::Add(float v)
{
    FromFloat(ToFloat() + v);
}


int FloatValue::Integer() const
{
    float abs = std::fabsf(ToFloat());

    return Sign() * static_cast<int>(abs);
}


int FloatValue::Fract(int numDigits) const
{
    float fract = ToFloat() - Integer();

    return static_cast<int>(fract * Math::Pow10(numDigits));
}

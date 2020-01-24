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


FloatValue::FloatValue(int units, uint mUnits, uint uUnits, uint nUnits)
{
    int sign = 1;

    if(units < 0)
    {
        sign = -1;
        units = -units;
    }

    value = static_cast<uint>(units);
    value *= 1000 * 1000 * 1000;
    
    value += nUnits + uUnits * 1000 + mUnits * 1000 * 1000;

    if(sign < 0)
    {
        SetSign(sign);
    }
}


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
        value &= 0x7FFFFFFFFFFFFFFFU;       /// \todo как это может работать?
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
    uint64 val = Abs();

    return static_cast<int>(val / (1000 * 1000 * 1000)) * Sign();
}


int FloatValue::Fract(int numDigits) const
{
    float fract = ToFloat() - Integer();

    return static_cast<int>(fract * ::Math::Pow10(numDigits));
}


int FloatValue::FractNano() const
{
    FloatValue val = *this;
    val.SetSign(1);

    int whole = val.Integer();

    return static_cast<int>(val.value - whole * 1000 * 1000 * 1000);
}


void FloatValue::Add(FloatValue add)
{
    int sign = Sign();
    int signAdd = add.Sign();

    SetSign(1);
    add.SetSign(1);

    if(sign > 0 && signAdd > 0)
    {
        value += add.value;
    }
    else if(sign < 0 && signAdd < 0)
    {
        value += add.value;
        SetSign(-1);
    }
    else if(sign > 0 && signAdd < 0)
    {
        if(value >= add.value)
        {
            value -= add.value;
        }
        else
        {
            value = add.value - value;
            SetSign(-1);
        }
    }
    else
    {
        if(add.value >= value)
        {
            value = add.value - value;
        }
        else
        {
            value -= add.value;
            SetSign(-1);
        }
    }
}


bool FloatValue::operator<(const FloatValue &rhs)
{
    return ToFloat() < rhs.ToFloat();
}


bool FloatValue::operator>(const FloatValue &rhs)
{
    return ToFloat() > rhs.ToFloat();
}

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
    FillBufferFromNano(static_cast<uint64>(std::fabsf(v) * 1E9F), v < 0.0F);
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
    //                     fedcba9876543210
    return (ToUINT64() & 0x7fffffffffffffff);
}


void FloatValue::DividePow10(uint)
{
//    int s = Sign();
//
//    SetSign(1);
//
//    value.dword /= div;
//
//    if(s < 0)
//    {
//        SetSign(-1);
//    }
}


void FloatValue::MultipliePow10(uint)
{
//    int sign = Sign();
//
//    SetSign(1);
//
//    value.dword *= mul;
//
//    if(sign < 0)
//    {
//        SetSign(-1);
//    }
}


void FloatValue::SetSign(int)
{
//    if (sign > 0)
//    {
//        value.word1 &= 0x7fffffff;     // ќбнул€ем признак отрицательного числа
//    }
//    else
//    {
//        //                76543210
//        value.word1 |= 0x80000000U;    // ”станавливаем признак отрицательного числа
//    }
}


void FloatValue::Add(float v)
{
    FromFloat(ToFloat() + v);
}


pString FloatValue::ToStringDigits(Order, int) const
{
//    static char buffer[40];
//
//    int sign = Sign();
//
//    uint intPart = value.word1 & 0x7fffffff;
//    uint fractPart = value.word0;
//
//    if(order == Order::Nano)
//    {
//
//    }
//    else if(order == Order::Micro)
//    {
//
//    }
//    else if(order == Order::Milli)
//    {
//
//    }
//    else if(order == Order::One)
//    {
//
//    }
//    else if(order == Order::Kilo)
//    {
//        fractPart = (fractPart / 1000) + 
//
//        intPart /= 1000;
//
//    }
//    else if(order == Order::Mega)
//    {
//        intPart /= (1000 * 1000);
//    }

    return "";
}


void FloatValue::FromUINT64(uint64)
{

}


uint64 FloatValue::ToUINT64() const
{
    return 0;
}


uint FloatValue::Integer() const
{
    return 0;
}


void FloatValue::FillBufferFromNano(uint64 value, bool negative)
{

}

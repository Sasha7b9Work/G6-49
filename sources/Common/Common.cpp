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
        value |= 0x8000000000000000U;           // ”станавливаем признак отрицательного числа
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


int FloatValue::Math::GetPositionFirstDigit(const FloatValue &_value)
{
    FloatValue value = _value;
    value.SetSign(1);

    int result = 0;

    if(value.Integer() > 0)
    {
        int whole = value.Integer();        // ÷ела€ часть числа

        while(whole > 9)
        {
            whole /= 10;
            result++;
        }
    }
    else
    {
        int fract = value.FractNano();
        
        if(fract == 0)
        {
            return 0;
        }

        do 
        {
            result--;
            fract *= 10;
        } while (fract < (1000 * 1000 * 1000));
    }

    return result;
}


int FloatValue::Math::GetDigit(const FloatValue &_value, int position)
{
    FloatValue value = _value;
    value.SetSign(1);

    if(position < 0)
    {
        int divider = 100 * 1000 * 1000;       /// Ќа это число будем делить количество наносекунд

        int fract = value.FractNano();

        while(position < -1)
        {
            divider /= 10;
            fract %= divider;
            position++;
        }

        return value.FractNano() / divider;
    }
    else
    {
        int whole = value.Integer();

        while(position > 0)
        {
            whole /= 10;
            position--;
        }

        return (whole % 10);
    }
}


pString FloatValue::GetStringDigits(int numDigits) const
{
    static char buffer[20];

    buffer[numDigits + 1] = '\0';

    int position = FloatValue::Math::GetPositionFirstDigit(*this);

    int posComma = FloatValue::Math::PositionComma(position);

    for(int i = 0; i <= numDigits; i++)
    {
        if(i == posComma)
        {
            buffer[i] = '.';
        }
        else
        {
            buffer[i] = static_cast<char>(FloatValue::Math::GetDigit(*this, position)) | 0x30;
            position--;
        }
    }

    return buffer;

}


int FloatValue::Math::PositionComma(int posFirstDigit)
{
    int result = posFirstDigit - 5;

    while(result < 1)
    {
        result += 3;
    }

    return result;
}

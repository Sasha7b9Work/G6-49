#include "stdafx.h"
#ifndef WIN32
#include "BigNumber.h"
#include "Utils/StringUtils.h"
#include "Utils/Math.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BigNumber::Number BigNumber::integer;
BigNumber::Number BigNumber::fract;
char BigNumber::sign = ' ';



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BigNumber::Set(char integer_[SIZE], char fract_[SIZE], char sign_)
{
    FillValue(integer_, integer);
    integer.numSymbols++;           // ƒобавл€ем один символ, чтобы было место дл€ возможного переполнени€
    FillValue(fract_, fract);
    sign = sign_;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BigNumber::FillValue(char buffer[SIZE], Number &number)
{
    number.Clear();

    char *pos = &buffer[SU::FindSymbol(buffer, '\0') - 1];  // Ќаходим адрес последнего символа в строке

    int pow = 1;

    while (pos >= buffer)
    {
        number.AddValue(*pos, pow);
        pow *= 10;
        pos--;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BigNumber::ChangeDigit(int position, Step step)
{
    if (position < 0)
    {
        position = -position - 1;                       // ѕересчитываем позицию дл€ отсчЄта положительными числами с конца
        ChangeInteger(position, step);
    }
    else
    {
        position = fract.numSymbols - position;         // ѕересчитываем позицию дл€ отсчЄта положительными числами с конца
        ChangeFract(position, step);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BigNumber::ChangeInteger(int position, Step step)
{
    if (sign == '-')
    {
        ChangeIntegerNegative(position, step);
    }
    else
    {
        ChangeIntegerPositive(position, step);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BigNumber::ChangeFract(int position, Step step)
{
    if (sign == '-')
    {
        ChangeFractNegative(position, step);
    }
    else
    {
        ChangeFractPositive(position, step);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BigNumber::ChangeIntegerPositive(int position, Step step)
{
    if (step.IsPlus())
    {
        ChangeIntegerPositivePlus(position);
    }
    else
    {
        ChangeIntegerPositiveMinus(position);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BigNumber::ChangeIntegerPositivePlus(int position)
{
    integer.value += Pow10(position);                       // ѕросто прибавл€ем одно значение, соответствующее данному разр€ду
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BigNumber::ChangeIntegerNegative(int position, Step step)
{
    if (step.IsPlus())
    {
        ChangeIntegerNegativePlus(position);
    }
    else
    {
        ChangeIntegerNegativeMinus(position);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BigNumber::ChangeIntegerNegativeMinus(int position)
{
    integer.value += Pow10(position);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BigNumber::ChangeFractPositive(int position, Step step)
{
    if(step.IsPlus())
    {
        ChangeFractPositivePlus(position);
    }
    else
    {
        ChangeFractPositiveMinus(position);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BigNumber::ChangeIntegerNegativePlus(int position)
{
    if (integer.value == 0 && fract.value == 0)
    {
        integer.value = Pow10(position);
        sign = '+';
    }
    else if (integer.value >= Pow10(position))
    {
        integer.value -= Pow10(position);
    }
    else
    {
        integer.value = fract.value = 0;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BigNumber::ChangeIntegerPositiveMinus(int position)
{
    if (integer.value == 0 && fract.value == 0)             // ≈сли нулевое значение, то прибавл€ем значение, соответствующее данному разр€ду.
    {                                                       // » мен€ем знак на минус. Ёто будет означать уменьшение
        integer.value = Pow10(position);
        sign = '-';
    }
    else if (integer.value >= Pow10(position))          // ≈сли можем спокойно отн€ть
    {
        integer.value -= Pow10(position);               // —покойно отнимаем
    }
    else                                                    // Ёто выбираетс€, когда должны перескочить в другой знак
    {
        integer.value = fract.value = 0;                    // ¬ этом случае просто обнул€ем число
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BigNumber::ChangeFractNegative(int position, Step step)
{
    if(step.IsPlus())
    {
        ChangeFractNegativePlus(position);
    }
    {
        ChangeFractNegativeMinus(position);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BigNumber::ChangeFractPositivePlus(int position)
{
    fract.value += Pow10(position);
    if (fract.value > fract.Max())
    {
        ChangeInteger(0, Step::Plus);
        fract.value -= (fract.Max() + 1);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BigNumber::ChangeFractNegativeMinus(int position)
{
    fract.value += Pow10(position);
    if(fract.value > fract.Max())
    {
        ChangeInteger(0, Step::Minus);
        fract.value -= (fract.Max() + 1);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BigNumber::ChangeFractPositiveMinus(int position)
{
    if (integer.value == 0 && fract.value == 0)
    {
        fract.value += Pow10(position);
        sign = '-';
    }
    else if (fract.value >= Pow10(position))
    {
        fract.value -= Pow10(position);
    }
    else
    {
        if (integer.value == 0)
        {
            fract.value = 0;
        }
        else
        {
            ChangeInteger(0, Step::Minus);
            fract.value = fract.Max() + 1 - Pow10(position);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BigNumber::ChangeFractNegativePlus(int position)
{
    if(integer.value == 0 && fract.value == 0)
    {
        fract.value += Pow10(position);
        sign = '+';
    }
    else if(fract.value >= Pow10(position))
    {
        fract.value -= Pow10(position);
    }
    else
    {
        if(integer.value == 0)
        {
            fract.value = 0;
        }
        else
        {
            ChangeInteger(0, Step::Plus);
            fract.value = fract.Max() + 1 - Pow10(position);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
float BigNumber::GetValue()
{
    float result = (float)integer.value + (float)fract.value / Pow10(fract.numSymbols);

    float mul = (sign == '-') ? -1.0f : 1.0f;

    return result * mul;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void BigNumber::Get(char /*integer_*/[SIZE], char /*fract_*/[SIZE], char * /*sign_*/)
{
    //*sign_ = sign;

}

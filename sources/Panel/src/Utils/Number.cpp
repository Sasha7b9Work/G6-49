#include "defines.h"
#include "Number.h"
#include "Utils/Math.h"


char *Number::integer = 0;
char *Number::fract = 0;
char  Number::sign = ' ';



void Number::Set(char integer_[SIZE_PART], char fract_[SIZE_PART], char sign_)
{
    integer = integer_;
    fract = fract_;
    sign = sign_;
    if(sign == ' ')
    {
        sign = '+';
    }
}


float Number::GetValue()
{
    uint64 intValue = GetInteger(integer);
    uint64 fractValue = GetInteger(fract);

    float result = static_cast<float>(intValue) + static_cast<float>(fractValue) / Math::Pow10(SIZE_PART);

    if(sign == '-')
    {
        result *= -1.0F;
    }

    return result;
}


uint64 Number::GetInteger(pCHAR buffer)
{
    uint64 result = 0;

    uint64 pow = 1;

    for(int i = SIZE_PART - 1; i >=0; i--)
    {
        result += (buffer[i] - 0x30) * pow;
        pow *= 10;
    }

    return result;
}


void Number::ChangeDigit(int position, Step step)
{
    if(sign == '-')
    {
        if(position < 0)
        {
            ChangeInNegativeInteger(position, step);
        }
        else
        {
            ChangeInNegativeFract(position, step);
        }
    }
    {
        if(position < 0)
        {
            ChangeInPositiveInteger(position, step);
        }
        else
        {
            ChangeInPositiveFract(position, step);
        }
    }
}


void Number::ChangeInPositiveInteger(int position, Step step)
{
    int pos = SIZE_PART + position;

    if(step == Step::Plus)
    {
        integer[pos]++;

        if(integer[pos] > '9')
        {
            integer[pos] = '0';
            ChangeDigit(position - 1, Step::Plus);
        }
    }
    else
    {
        if(integer[pos] == '0')
        {
            if(AllLeftZeroInInteger(position))
            {
                if(GetValue() == 0.0F) //-V550 //-V2550
                {
                    integer[pos] = 1;
                    sign = '-';
                }
                else
                {
                    SetAllZeros();
                }
            }
            else
            {
                integer[pos] = '9';
                ChangeDigit(position - 1, Step::Minus);
            }
        }
        else
        {
            integer[pos]--;
        }
    }
}


void Number::ChangeInNegativeInteger(int /*position*/, Step /*step*/)
{
//    int pos = SIZE_PART + position;

    /*
    if(step == Step::Plus)
    {

    }
    else
    {
    }
    */
}


void Number::ChangeInPositiveFract(int /*position*/, Step /*step*/)
{
}


void Number::ChangeInNegativeFract(int /*position*/, Step /*step*/)
{
}


void Number::SetAllZeros()
{

}


bool Number::AllLeftZeroInInteger(int /*position*/)
{
    return false;
}

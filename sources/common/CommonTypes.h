#pragma once
#include "defines.h"

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define COMMON_ENUM     uint8 value; operator uint8() const { return value; }; operator uint8() { return value; }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Chan
{
    enum
    {
        A,
        B,
        Number
    };
    COMMON_ENUM;
    Chan(uint8 v = A) : value(v)
    {
    };
    Chan(int v) : value((uint8)v)
    {
    }
    bool IsA() const
    {
        return value == A;
    }
    bool IsB() const
    {
        return value == B;
    }
};

/// Сопротивление входа частотомера
enum FreqResist
{
    FreqResist_1MOhm,
    FreqResist_50Ohm
};

/// Открытый/закрытый вход частотомера
enum FreqCouple
{
    FreqCouple_AC,
    FreqCouple_DC
};

/// ФНЧ частотомера
enum FreqFiltr
{
    FreqFiltr_Enable,
    FreqFiltr_Disable
};

#ifdef WIN32
#pragma warning(pop)
#endif

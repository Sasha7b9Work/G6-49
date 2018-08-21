#pragma once
#include "defines.h"

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#undef COMMON_ENUM
#define COMMON_ENUM     \
    uint8 value; operator uint8() const { return value; }; operator uint8() { return value; }; bool Is(uint8 v) const { return v == value; }


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
struct FreqResist
{
    enum
    {
        _1MOhm,
        _50Ohm
    };
    COMMON_ENUM;
    FreqResist(uint v) : value((uint8)v)
    {
    };
    FreqResist(int v) : value((uint8)v)
    {
    };
};

/// Открытый/закрытый вход частотомера
struct FreqCouple
{
    enum
    {
        AC,
        DC
    };
    COMMON_ENUM;
    FreqCouple(uint v) : value((uint8)v)
    {
    };
    FreqCouple(int v) : value((uint8)v)
    {
    };
};

/// ФНЧ частотомера
struct FreqFiltr
{
    enum
    {
        Enable,
        Disable
    };
    COMMON_ENUM;
    FreqFiltr(uint v) : value((uint8)v)
    {
    };
    FreqFiltr(int v) : value((uint8)v)
    {
    };
};

#ifdef WIN32
#pragma warning(pop)
#endif

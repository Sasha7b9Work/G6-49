#pragma once
#include "defines.h"

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Chan
{
    enum E
    {
        A,
        B,
        Number
    } value;
    Chan(E v) : value(v) {};
    Chan(int v) : value((E)v) {};
    bool IsA() const { return value == A; };
    bool IsB() const { return value == B; };
    operator uint8() const { return (uint8)value; };
};

/// Сопротивление входа частотомера
struct FreqResist
{
    enum E
    {
        _1MOhm,
        _50Ohm
    } value;
    bool Is(E v) const { return value == v; };
    FreqResist(E v) : value(v) {};
    FreqResist(int v) : value((E)v) {};
    operator uint() const { return (uint)value; };
};

/// Открытый/закрытый вход частотомера
struct FreqCouple
{
    enum E
    {
        AC,
        DC
    } value;
    FreqCouple(E v) : value(v) {};
    FreqCouple(int v) : value((E)v) {};
    bool Is(E v) const { return v == value; };
    operator uint() const { return (uint)value; };
};

/// ФНЧ частотомера
struct FreqFiltr
{
    enum E
    {
        Enable,
        Disable
    } value;
    FreqFiltr(E v) : value(v) {};
    FreqFiltr(int v) : value((E)v) {};
    bool Is(E v) const { return v == value; };
    operator uint() const { return (uint)value; };
};

#ifdef WIN32
#pragma warning(pop)
#endif

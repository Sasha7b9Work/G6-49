#pragma once
#include "defines.h"

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4514)
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Control
{
    enum E
    {
        None      = 0x00,
        F1        = 0x01,
        F2        = 0x02,
        F3        = 0x03,
        F4        = 0x04,
        _0        = 0x05,
        _1        = 0x06,
        _2        = 0x07,
        _3        = 0x08,
        _4        = 0x09,
        _5        = 0x0a,
        _6        = 0x0b,
        _7        = 0x0c,
        _8        = 0x0d,
        _9        = 0x0e,
        Dot       = 0x0f,
        Minus     = 0x10,
        Esc       = 0x11,
        Left      = 0x12,
        Right     = 0x13,
        On1       = 0x15,
        On2       = 0x15
    } value;

    struct Reg
    {
        enum E
        {
            Left   = 0x16,
            Right  = 0x17,
            Button = 0x18
        };
    };

    struct Action
    {
        enum E
        {
            Down,
            Long,
            Up
        };
        E value;
        Action(E v = Down) : value(v) {};
        operator uint8() const { return (uint8)value; };
        bool Is(Action a) const { return a.value == value; };
        /// Возвращает true, если Up или Long
        bool IsRelease() const;
    } action;

    Control(E v = None, Action::E a = Action::Down) : value(v), action(a) {};
    Control(Reg::E reg) : value((Control::E)reg) {};
    Control operator=(Control rval)
    {
        value = rval.value;
        action = rval.action;
        return *this;
    }
    operator uint8() const { return (uint8)value; };
    bool Is(Control c) { return c.value == value; };
    bool IsDigit() const;
    char ToChar() const;
    pString Name() const;
    bool IsFunctional() const;
    /// Возвращает true, если поворот ручки
    bool IsRotate() const;
    /// Возвращает true, если кнопка управления курсором (Влево-Вправо)
    bool IsCursors() const { return value == Left || value == Right; };
};


#ifdef WIN32
#pragma warning(pop)
#endif

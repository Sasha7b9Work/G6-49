#pragma once
#include "defines.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct KeyEvent
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
        On1       = 0x14,
        On2       = 0x15,
        RegLeft   = 0x16,
        RegRight  = 0x17,
        RegButton = 0x18
    } value;

    struct Action
    {
        enum E
        {
            Down,
            Long,
            Up
        } value;
        Action(E v = Down) : value(v) {};
        operator uint8() const { return (uint8)value; };
        bool Is(Action a) const { return a.value == value; };
        /// Возвращает true, если Up или Long
        bool IsRelease() const;
    } action;

    explicit KeyEvent(E v = None, Action::E a = Action::Down) : value(v), action(a) {};
    KeyEvent &operator=(KeyEvent rval)
    {
        value = rval.value;
        action = rval.action;
        return *this;
    }
    KeyEvent(const KeyEvent &c)
    {
        value = c.value;
        action = c.action;
    }
    operator uint8() const { return (uint8)value; };
    bool Is(KeyEvent::E c) { return c == value; };
    bool Is(KeyEvent::E c, Action::E a) { return value == c && action == a; };
    bool IsDigit() const;
    char ToChar() const;
    pString Name() const;
    bool IsFunctional() const;
    /// Возвращает true, если поворот ручки
    bool IsRotate() const;
    /// Возвращает true, если кнопка управления курсором (Влево-Вправо)
    bool IsCursors() const { return value == Left || value == Right; };
};

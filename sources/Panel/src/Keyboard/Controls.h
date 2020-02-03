#pragma once


struct Key
{
    enum E
    {
        None,
        F1,
        F2,
        F3,
        F4,
        _0,
        _1,
        _2,
        _3,
        _4,
        _5,
        _6,
        _7,
        _8,
        _9,
        Dot,
        Minus,
        Esc,
        Left,
        Right,
        On1,
        On2,
        RegLeft,
        RegRight,
        RegButton,
        Count
    } value;

    struct Action
    {
        enum E
        {
            Down,
            Long,
            Up,
            Count
        } value;
        Action(E v = Down) : value(v) {};
        operator uint8() const { return static_cast<uint8>(value); };
        bool Is(Action a) const { return a.value == value; };
        /// Возвращает true, если Up или Long
        bool IsRelease() const;
    } action;

    Key(E v = None, Action::E a = Action::Down) : value(v), action(a) {};
    Key &operator=(Key rval)
    {
        value = rval.value;
        action = rval.action;
        return *this;
    }
    Key(const Key &c)
    {
        value = c.value;
        action = c.action;
    }
    operator uint8() const { return static_cast<uint8>(value); };
    bool Is(Key::E c) const { return (c == value); };
    bool Is(Key::E c, Action::E a) const { return value == c && action == a; };
    bool IsDigit() const;
    bool IsUp() const { return action == Action::Up; }
    bool IsLong() const { return action == Action::Long; }
    char ToChar() const;
    pString Name() const;
    bool IsFunctional() const;
    /// Возвращает true, если поворот ручки
    bool IsRotate() const;
    /// Возвращает true, если кнопка управления курсором (Влево-Вправо)
    bool IsCursors() const { return value == Left || value == Right; };
};

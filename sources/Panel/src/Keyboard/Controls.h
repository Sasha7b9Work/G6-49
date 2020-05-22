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
        RotateLeft,
        RotateRight,
        RegButton,
        Count
    };

    Key(E v) : value(v) { }

    char ToChar() const;

    bool IsDigit() const;

    pString Name() const;

    E value;
};


struct Action
{
    enum E
    {
        Down,
        Long,
        Up,
        Count
    };
};


struct Control
{
    Key::E key;
    Action::E action;

    bool Is(Key::E c) const           { return (c == key); };
    
    bool Is(Key::E c, Action::E a) const { return (key == c) && (action == a); };
       
    bool IsUp() const   { return (action == Action::Up); }
    
    bool IsDown() const { return (action == Action::Down); }
    
    bool IsLong() const { return (action == Action::Long); }
   
    bool IsFunctional() const;
    
    // Возвращает true, если поворот ручки
    bool IsRotate() const;

    // Возвращает true, если кнопка управления курсором (Влево-Вправо)
    bool IsCursors() const { return (key == Key::Left) || (key == Key::Right); };

    // Возвращает true в случае отпускания кнопки или "длинного" нажатия
    bool IsRelease() const;

    bool operator==(const Control &rhl) const
    {
        return (rhl.key == key) && (rhl.action == action);
    }
};

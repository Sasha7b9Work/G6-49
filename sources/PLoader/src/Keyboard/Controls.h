// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
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

    enum Action
    {
        Down,
        Long,
        Up,
        NumActions
    } action;

    operator uint8() const            { return (uint8)value; };
    bool Is(Key::E c) const           { return (c == value); };
    bool Is(Key::E c, Action a) const { return value == c && action == a; };
    bool IsDigit() const;
    bool IsUp() const   { return action == Up; }
    bool IsDown() const { return action == Down; }
    bool IsLong() const { return action == Long; }
    char ToChar() const;
    static cstr Name(Key::E key);
    bool IsFunctional() const;
    // ���������� true, ���� ������� �����
    bool IsRotate() const;
    // ���������� true, ���� ������ ���������� �������� (�����-������)
    bool IsCursors() const { return value == Left || value == Right; };
    // ���������� true � ������ ���������� ������ ��� "��������" �������
    bool IsRelease() const;
};

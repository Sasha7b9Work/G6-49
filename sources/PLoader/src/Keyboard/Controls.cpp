// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Controls.h"


bool Key::IsDigit() const
{
    return (value >= _0 && value <= _9);
}


char Key::ToChar() const
{
    if(IsDigit())
    {
        return (char)((value - _0) | 0x30);
    }
    else if(value == Dot)
    {
        return '.';
    }

    return '\0';
}


bool Key::IsFunctional() const
{
    return (value >= F1) && (value <= F4);
}


bool Key::IsRotate() const
{
    return (value == RegLeft) || (value == RegRight);
}


bool Key::IsRelease() const
{
    return (action == Up) || (action == Long);
}


cstr Key::Name(Key::E key)
{
    static pchar names[] =
    {
        "None",
        "F1",
        "F2",
        "F3",
        "F4",
        "0",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        ".",
        "-",
        "Esc",
        "Left",
        "Right",
        "On1",
        "On2",
        "RegLeft",
        "RegRight",
        "RegButton"
    };

    return names[key];
}

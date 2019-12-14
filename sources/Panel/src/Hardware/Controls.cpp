#include "Controls.h"


bool KeyEvent::IsDigit() const
{
    return (value >= _0 && value <= _9);
}


char KeyEvent::ToChar() const
{
    if(IsDigit())
    {
        return static_cast<char>((value - _0) | 0x30);
    }
    else if(value == Dot)
    {
        return '.';
    }
    else
    {
        // здесь ничего
    }

    return '\0';
}


bool KeyEvent::IsFunctional() const
{
    return value >= F1 && value <= F4;
}


bool KeyEvent::IsRotate() const
{
    return value == RegLeft || value == RegRight;
}


bool KeyEvent::Action::IsRelease() const
{
    return value == Up || value == Long;
}


pString KeyEvent::Name() const
{
    static const char *names[] =
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

    return names[value];
}

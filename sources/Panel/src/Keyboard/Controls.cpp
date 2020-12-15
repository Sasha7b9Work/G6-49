#include "defines.h"
#include "Controls.h"


bool Key::IsDigit() const
{
    return (value >= Key::_0 && value <= Key::_9);
}


char Key::ToChar() const //-V2506
{
    if(IsDigit())
    {
        return static_cast<char>((value - Key::_0) | 0x30);
    }
    else if(value == Key::Comma)
    {
        return '.';
    }
    else if (value == Key::Minus) //-V2516
    {
        return '-';
    }

    return ' ';
}


bool Control::IsFunctional() const
{
    return (key >= Key::F1) && (key <= Key::F4);
}


bool Control::IsRotate() const
{
    return (key == Key::RotateLeft) || (key == Key::RotateRight);
}


bool Control::IsRelease() const
{
    return (action == Action::Up) || (action == Action::Long);
}


pString Key::Name() const
{
    static pCHAR names[] =
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


bool Control::IsEntering() const //-V2506
{
    if (key >= Key::_0 && key <= Key::_9)
    {
        return true;
    }

    return ((key == Key::Minus) || (key == Key::Comma));
}

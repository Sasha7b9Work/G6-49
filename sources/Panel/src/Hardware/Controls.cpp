#include "Controls.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool KeyIsDigit(Control key)
{
    return (key >= Control::B_0 && key <= Control::B_9);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
char KeyToChar(Control key)
{
    if(KeyIsDigit(key))
    {
        return (char)((key - Control::B_0) | 0x30);
    }
    else if(key == Control::B_Dot)
    {
        return '.';
    }

    return '\0';
}

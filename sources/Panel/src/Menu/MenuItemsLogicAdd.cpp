#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Button_Press(Button *button)
{
    if (button && button->funcOnPress)
    {
        button->funcOnPress();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SButton_Press(SButton *button)
{
    if (button && button->funcOnPress)
    {
        button->funcOnPress();
    }
}

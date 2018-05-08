#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ChoiceWaveParameter_Press(ChoiceParameter *choice, TypePress press)
{
    if (press == TypePress_Release)
    {
        volatile bool allow = false;
        do
        {
            CircleIncrease<uint8>(choice->numParameter, 0, (uint8)(NumParameters - 1));
            allow = (choice->allowParameters).allow[*choice->numParameter];
        } while (!allow);
    }
    else if (press == TypePress_LongPress)
    {
        OPENED_ITEM = choice;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Item_Rotate(Control *item, PanelControl control)
{
    if (item->IsChoice())
    {
        Choice *choice = (Choice*)item;

        if (control == REG_LEFT)
        {
            CircleIncrease<int8>(choice->cell, 0, (int8)(choice->NumSubItems() - 1));
        }
        else if (control == REG_RIGHT)
        {
            if(*(choice->cell) == 0)
            {
                *(choice->cell) = (int8)(choice->NumSubItems() - 1);
            }
            else
            {
                --(*(choice->cell));
            }
        }
        if (choice->funcOnChanged)
        {
            choice->funcOnChanged(true);
        }
    }
}

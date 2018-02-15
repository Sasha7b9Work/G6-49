#include "MenuItemsLogic.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Choice_Press(Choice *choice, TypePress press)
{
    if (press == TypePress_Release)
    {
        CIRCLE_INCREASE(*(choice->cell), 0, Choice_NumChoices(choice) - 1);
        if (choice->funcOnPress)
        {
            choice->funcOnPress();
        }
    }
    else if (press == TypePress_LongPress)
    {
        OPENED_ITEM = choice;
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ChoiceWaveParameter_Press(ChoiceParameter *choice, TypePress press)
{
    if (press == TypePress_Release)
    {
        volatile bool allow = false;
        do
        {
            CIRCLE_INCREASE(*(choice->numParameter), 0, (int)NumParameters - 1);
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
void Item_Rotate(void *item, Control control)
{
    if (ItemIsChoice(item))
    {
        Choice *choice = (Choice*)item;

        if (control == REG_A_LEFT)
        {
            CIRCLE_INCREASE(*(choice->cell), 0, Choice_NumChoices(choice) - 1);
        }
        else if (control == REG_B_RIGHT)
        {
            if(*(choice->cell) == 0)
            {
                *(choice->cell) = (uint8)(Choice_NumChoices(choice) - 1);
            }
            else
            {
                --(*(choice->cell));
            }
        }
        if (choice->funcOnPress)
        {
            choice->funcOnPress();
        }
    }
}

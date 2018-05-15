#include "MenuItemsAdd.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include <ctype.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool ItemIsButton(void* item)
{
    return item && (((Control *)item)->Type() == Item_Button);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool ItemIsSButton(void* item)
{
    return item && (((Control *)item)->Type() == Item_SmallButton);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool ItemIsChoiceParameter(void *item)
{
    return item && (((Control *)item)->Type() == Item_ChoiceParameter);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
PanelControl ButtonForItem(void *item)
{
    int pos = PositionOnPage(item);

    if (pos >= 0)
    {
        return (PanelControl)((int)B_F1 + pos);
    }

    return B_None;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *ItemTitle(void* item)
{
    return ((Choice*)item)->titleHint[LANG];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Choice_CurrentChoice(Choice *choice)
{
    TypeItem type = choice->Type();

    int retValue = 0;

    if (type == Item_Choice)
    {
        retValue = *(choice->cell);
    }
    else if (type == Item_ChoiceParameter)
    {
        ChoiceParameter *param = (ChoiceParameter *)choice;

        for (int i = 0; i < NumParameters; i++)
        {
            if ((param->allowParameters).allow[i] == false)
            {
                continue;
            }
            if (i == *param->numParameter)
            {
                break;
            }
            retValue++;
        }
    }

    return retValue;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int PositionOnPage(void *item)
{
    for (int numPage = 0; numPage < NUM_PAGES; numPage++)
    {
        for (int numItem = 0; numItem < NUM_ITEMS_ON_PAGE; numItem++)
        {
            if (item == menu[numPage]->items[numItem])
            {
                return numItem;
            }
        }
    }
    return -1;
}

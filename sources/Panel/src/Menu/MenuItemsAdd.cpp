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

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
PanelControl ButtonForItem(Control *item)
{
    int pos = item->PositionOnPage();

    if (pos >= 0)
    {
        return (PanelControl)((int)B_F1 + pos);
    }

    return B_None;
}

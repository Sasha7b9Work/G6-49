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

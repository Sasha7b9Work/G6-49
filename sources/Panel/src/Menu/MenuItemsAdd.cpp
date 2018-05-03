#include "MenuItemsAdd.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void* ItemFromPage(int numItem)
{
    Page *page = CurrentPage();

    if(page->NumItems() - 1 < numItem)
    {
        return 0;
    }

    return (void *)page->items[numItem];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool ItemIsButton(void* item)
{
    return item && (TypeOfItem(item) == Item_Button);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool ItemIsChoice(void* item)
{
    return item && (TypeOfItem(item) == Item_Choice);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool ItemIsSButton(void* item)
{
    return item && (TypeOfItem(item) == Item_SmallButton);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
bool ItemIsChoiceParameter(void *item)
{
    return item && (TypeOfItem(item) == Item_ChoiceParameter);
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
    TypeItem type = TypeOfItem(choice);

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

//----------------------------------------------------------------------------------------------------------------------------------------------------
TypeItem TypeOfItem(void *address)
{
    if (address)
    {
        return ((Choice*)address)->type;
    }
    return Item_None;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Page* CurrentPage(void)
{
    volatile int8 num = CURRENT_PAGE;

    if(num == 1)
    {
        num = num;
    }
    
    Page *page = (Page *)menu[num];

    return (ADDITION_PAGE_IS_NONE ? page : ADDITION_PAGE);
}

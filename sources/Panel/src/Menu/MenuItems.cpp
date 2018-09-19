#include "MenuItems.h"
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include <Menu/Menu.h>
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Log.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NAME_FROM_INDEX(index) (names[index * 2 + LANG])


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *Choice::NameCurrentSubItem() const
{
    return NAME_FROM_INDEX(CurrentIndex());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *Choice::NameNextSubItem() const
{
    int8 nextIndex = CurrentIndex() + 1;

    if (nextIndex == NumSubItems())
    {
        nextIndex = 0;
    }
    return NAME_FROM_INDEX(nextIndex);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *Choice::NamePrevSubItem() const
{
    int8 prevIndex = CurrentIndex() - 1;

    if (prevIndex < 0)
    {
        prevIndex = NumSubItems() - 1;
    }
    return NAME_FROM_INDEX(prevIndex);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
Item *Page::GetItem(Control key) const
{
    return (Item *)items[CurrentSubPage() * Menu::NUM_ITEMS_ON_DISPLAY + (key - Control::F1)];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Page::PosItemOnTop() const
{
    return CurrentSubPage() * Menu::NUM_ITEMS_ON_DISPLAY;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
const char *Choice::NameSubItem(int i) const
{
    return NAME_FROM_INDEX(i);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Page::ShortPressOnItem(int numItem)
{
    ((Button *)items[numItem])->funcOnPress();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int8 Page::PosCurrentItem() const
{
    return MENU_POS_ACT_ITEM(nameOrNumBit) & 0x7f;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Page::ChangeSubPage(int delta)
{
    if (delta > 0 && CurrentSubPage() < NumSubPages() - 1)
    {
        SetCurrentSubPage(CurrentSubPage() + 1);
    }
    else if (delta < 0 && CurrentSubPage() > 0)
    {
        SetCurrentSubPage(CurrentSubPage() - 1);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Page::ProcessingControl(Control control)
{
    if(fuuncOnKey)
    {
        return fuuncOnKey(control);
    }
    
    return false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Item::IsShade() const
{
    return Menu::GetOpenedItem() &&  this != Menu::GetOpenedItem();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Item::IsPressed() const
{
    return this == Menu::pressedItem;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Item::IsOpened() const
{
    if (type == Item::Type::Page)
    {
        return keeper->CurrentItemIsOpened();
    }
    return Menu::GetOpenedItem() == this;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Item::Open(bool open)
{
    Page *parent = (Page *)keeper;
    parent->SetPosActItem(open ? (parent->PosCurrentItem() | 0x80) : (parent->PosCurrentItem() & 0x7f));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
pString Item::Title() const
{
    return titleHint[LANG];
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
#define CURRENT_ITEM_IS_OPENED return _GET_BIT(MENU_POS_ACT_ITEM(nameOrNumBit), 7) == 1

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool PageBase::CurrentItemIsOpened() const
{
    CURRENT_ITEM_IS_OPENED;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
bool Page::CurrentItemIsOpened() const
{
    CURRENT_ITEM_IS_OPENED;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Page::CurrentSubPage() const
{
    return MENU_CURRENT_SUBPAGE(nameOrNumBit);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Page::SetPosActItem(int pos)
{
    MENU_POS_ACT_ITEM(nameOrNumBit) = (int8)pos;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Page::SetCurrentSubPage(int pos)
{
    MENU_CURRENT_SUBPAGE(nameOrNumBit) = (int8)pos;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Item::DrawHint(int x, int y, int width, Color color) const
{
    Painter::SetColor(color);

    return Text::DrawTextInColumnWithTransfers(x, y, width, titleHint[2 + LANG], color);
}

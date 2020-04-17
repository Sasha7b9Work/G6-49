#include "MenuItems.h"
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include <Menu/Menu.h>
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Log.h"


#define NAME_FROM_INDEX(index, lang) ((lang) ? namesEN[index] : namesRU[index])


static Item emptyLight = { Item::Type::NoneLight };
static Item emptyDark = { Item::Type::NoneDark };


Item *Item::EmptyLight()
{
    return &emptyLight;
}


Item *Item::EmptyDark()
{
    return &emptyDark;
}


const char *Choice::NameCurrentSubItem(uint lang) const
{
    return NAME_FROM_INDEX(CurrentIndex(), lang);
}


String Choice::NameNextSubItem(uint lang) const
{
    int8 nextIndex = CurrentIndex() + 1;

    if (nextIndex == NumSubItems())
    {
        nextIndex = 0;
    }
    return String(NAME_FROM_INDEX(nextIndex, lang));
}


const char *Choice::NamePrevSubItem(uint lang)
{
    int8 prevIndex = CurrentIndex() - 1;

    if (prevIndex < 0)
    {
        prevIndex = NumSubItems() - 1;
    }
    return NAME_FROM_INDEX(prevIndex, lang);
}


Item *Page::GetItem(const Key &key) const
{
    int index = CurrentSubPage() * Menu::NUM_ITEMS_ON_DISPLAY + (key - Key::F1);

    return (index < NumItems()) ? const_cast<Item *>(items[index]) : Item::EmptyLight();
}


String Choice::NameSubItem(int i, uint lang) const
{
    return String(NAME_FROM_INDEX(i, lang));
}


int8 Page::PosCurrentItem() const
{
    return MENU_POS_ACT_ITEM(nameOrNumBit) & 0x7f;
}


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
    else
    {
        // здесь ничего
    }
}


bool Item::IsShade() const
{
    return Menu::GetOpenedItem() &&  this != Menu::GetOpenedItem();
}


bool Item::IsPressed() const
{
    return this == Menu::pressedItem;
}


bool Item::IsOpened() const
{
    if (type == Item::Type::Page)
    {
        return Keeper()->CurrentItemIsOpened();
    }
    return Menu::GetOpenedItem() == this;
}


void Item::Open(bool open)
{
    Keeper()->SetPosActItem(open ? (Keeper()->PosCurrentItem() | 0x80) : (Keeper()->PosCurrentItem() & 0x7f));
}


String Item::GetTitle(uint lang) const
{
    return String(title[lang]);
}


#define CURRENT_ITEM_IS_OPENED return _GET_BIT(MENU_POS_ACT_ITEM(nameOrNumBit), 7) == 1


bool PageBase::CurrentItemIsOpened() const
{
    CURRENT_ITEM_IS_OPENED;
}


bool Page::CurrentItemIsOpened() const
{
    CURRENT_ITEM_IS_OPENED;
}


int Page::CurrentSubPage() const
{
    return MENU_CURRENT_SUBPAGE(nameOrNumBit);
}


void Page::SetPosActItem(int pos)
{
    MENU_POS_ACT_ITEM(nameOrNumBit) = static_cast<int8>(pos);
}


void Page::SetCurrentSubPage(int pos)
{
    MENU_CURRENT_SUBPAGE(nameOrNumBit) = static_cast<int8>(pos);
}


int Item::DrawHint(int x, int y, int width, Color color) const
{
    color.SetAsCurrent();

    return Text::DrawTextInColumnWithTransfers(x, y, width, hint[LANGUAGE], color);
}

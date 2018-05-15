#include "defines.h"
#include "Menu.h"
#include "Menu/MenuItems.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Draw()
{
    DrawTitle();

    for (int i = 0; i < 4; i++)
    {
        int x = SCREEN_WIDTH - ITEM_WIDTH - 1;
        int y = MP_TITLE_HEIGHT + i * MI_HEIGHT;
        Painter::DrawRectangle(x, y, ITEM_WIDTH, MI_HEIGHT);
        CurrentPage()->Item(i)->Draw(x, y, false);
    }

    DrawPagesUGO(); 

    if (OPENED_ITEM)
    {
        DrawOpenedItem();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::DrawTitle()
{
    Painter::DrawRectangle(0, 0, SCREEN_WIDTH - 1, MP_TITLE_HEIGHT, Color::FILL);
    Painter::DrawTextRelativelyRight(315, 5, CurrentPage()->Title());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::DrawPagesUGO()
{
    static const int delta = 10;

    if (RegIsControlPages())
    {
        for (int i = 0; i < NUM_PAGES; i++)
        {
            Painter::DrawRectangle(5 + i * delta, 5, delta, 8, Color::GRAY_75);
        }

        Painter::FillRegion(5 + CURRENT_PAGE * delta, 5, delta, 8, Color::FILL);

        char buffer[20];

        Painter::DrawText(6 + CURRENT_PAGE * delta, 5, Int2String(CURRENT_PAGE + 1, false, 2, buffer), Color::BACK);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::DrawOpenedItem()
{
    if (OPENED_ITEM)
    {
        TypeItem typeItem = ((Control *)OPENED_ITEM)->Type();

        if (typeItem == Item_Choice)
        {
            ((Choice *)OPENED_ITEM)->DrawOpened();
        }
        else if (typeItem == Item_ChoiceParameter)
        {
            ((Choice *)OPENED_ITEM)->DrawOpened();
        }
    }
}

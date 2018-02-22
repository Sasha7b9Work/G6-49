#include "defines.h"
#include "Menu.h"
#include "MenuItemsDrawing.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Draw(void)
{
    DrawTitle();

    for (int i = 0; i < 4; i++)
    {
        int x = SCREEN_WIDTH - ITEM_WIDTH - 1;
        int y = TITLE_HEIGHT + i * ITEM_HEIGHT;

        if (CurrentPage()->typeItem == Item_Page)
        {
            Painter::DrawRectangle(x, y, ITEM_WIDTH, ITEM_HEIGHT);
        }
        Item_Draw(x, y, ItemFromPage(i));
    }

    DrawPagesUGO();

    if (OPENED_ITEM)
    {
        DrawOpenedItem();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::DrawTitle(void)
{
    Painter::DrawRectangle(0, 0, SCREEN_WIDTH - 1, TITLE_HEIGHT, Color::FILL);
    Painter::DrawTextRelativelyRight(315, 5, Page_Name(CurrentPage()));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::DrawPagesUGO(void)
{
    if (RegIsControlPages())
    {
        for (int i = 0; i < NUM_PAGES; i++)
        {
            Painter::DrawRectangle(7 + i * 16, 7, 5, 5, Color::FILL);
        }

        Painter::FillRegion(5 + CURRENT_PAGE * 16, 5, 9, 9);
    }
}



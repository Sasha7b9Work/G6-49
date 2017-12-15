#include "defines.h"
#include "Menu.h"
#include "MenuItemsDrawing.h"
#include "Display/Painter.h"
#include "Display/PainterC.h"
#include "Display/PainterText.h"
#include "Settings/Settings.h"
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Draw(void)
{
    DrawTitle();

    for (int i = 0; i < 4; i++)
    {
        int x = WIDTH_SCREEN - WIDTH_ITEM - 1;
        int y = HEIGHT_TITLE + i * HEIGHT_ITEM;

        if (CurrentPage()->typeItem == Item_Page)
        {
            Painter_DrawRectangle(x, y, WIDTH_ITEM, HEIGHT_ITEM);
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
    Painter_DrawRectangleC(0, 0, WIDTH_SCREEN - 1, HEIGHT_TITLE, COLOR_FILL);
    Painter_DrawTextRelativelyRight(315, 5, Page_Name(CurrentPage()));
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::DrawPagesUGO(void)
{
    if (RegIsControlPages())
    {
        for (int i = 0; i < NUM_PAGES; i++)
        {
            Painter_DrawRectangleC(7 + i * 16, 7, 5, 5, COLOR_FILL);
        }

        Painter_FillRegion(5 + CURRENT_PAGE * 16, 5, 9, 9);
    }
}



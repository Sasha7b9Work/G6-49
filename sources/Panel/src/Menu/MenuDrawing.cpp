#include "defines.h"
#include "Menu.h"
#include "Menu/MenuItems.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Draw()
{
    DrawTitle();

    CURRENT_PAGE->Draw();

    if(OpenedItem())
    {
        OpenedItem()->Draw(true);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::DrawTitle()
{
    Painter::DrawRectangle(0, 0, SCREEN_WIDTH - 1, MP_TITLE_HEIGHT, Color::FILL);
    Painter::FillRegion(1, 1, SCREEN_WIDTH - 3, MP_TITLE_HEIGHT - 2, Color::BLUE_10);
    if(Menu::OpenedItem())
    {
        Text::DrawTextRelativelyRight(315, 5, Menu::OpenedItem()->FullPath(), Color::FILL);
    }
    else
    {
        DrawPagesUGO();
        if(CURRENT_PAGE)
        {
            Text::DrawTextRelativelyRight(315, 5, CURRENT_PAGE->Title(), Color::FILL);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::DrawPagesUGO()
{
    /*
    static const int delta = 10;

    if (RegIsControlSubPages())
    {
        for (int i = 0; i < NUM_PAGES; i++) 
        {
            Painter::DrawRectangle(5 + i * delta, 5, delta, 8, Color::GRAY_25);
        }

        Painter::FillRegion(5 + CURRENT_PAGE * delta, 5, delta, 8, Color::FILL);

        char buffer[20];

        Text::DrawText(6 + CURRENT_PAGE * delta, 5, Int2String(CURRENT_PAGE + 1, false, 2, buffer), Color::BACK);
    }
    */
}

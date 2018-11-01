#include "defines.h"
#include "Menu.h"
#include "Menu/MenuItems.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include "Utils/Debug.h"
#include "Display/InputWindow.h"
#include <math.h>


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Menu::Draw()
{
    int x = DrawTitle();
    DrawPagesUGO(x);
    CURRENT_PAGE->DrawOpened();
    if(GetOpenedItem())
    {
        GetOpenedItem()->Draw(true);
    }
    if(oldPage)
    {
        InputWindow::Draw();
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
int Menu::DrawTitle()
{
    Painter::DrawRectangle(0, 0, SCREEN_WIDTH - 1, Page::Title::HEIGHT, Color::FILL);
    Painter::FillRegion(1, 1, SCREEN_WIDTH - 3, Page::Title::HEIGHT - 2, Color::BLUE_10);
    if(Menu::GetOpenedItem())
    {
        return Text::DrawText(5, 5, Menu::GetOpenedItem()->FullPath(), Color::FILL);
    }

    return Text::DrawText(5, 5, CURRENT_PAGE->FullPath(), Color::FILL);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::DrawPagesUGO(int x)
{
    x += 10;

    static const int delta = 12;

    int numSubPages = CURRENT_PAGE->NumSubPages();
    int numPage = CURRENT_PAGE->CurrentSubPage();

    if (RegIsControlSubPages() && numSubPages > 1)
    {
        for (int i = 0; i < numSubPages; i++) 
        {
            Painter::DrawRectangle(x + 5 + i * delta, 5, delta, 9, Color::GRAY_25);
        }

        Painter::FillRegion(x + 5 + numPage * delta, 5, delta, 9, Color::FILL);

        char buffer[20];

        Text::DrawText(x + 6 + numPage * delta, 5, Int2String(numPage + 1, false, 2, buffer), Color::BACK);
    }
}

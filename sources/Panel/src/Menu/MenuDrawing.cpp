#include "defines.h"
#include "Menu.h"
#include "Menu/MenuItems.h"
#include "Display/Painter.h"
#include "Display/Text.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include <cmath>


using namespace Primitives;


void Menu::Draw()
{
    int x = DrawTitle();
    DrawPagesUGO(x);
    CURRENT_PAGE->DrawOpened();
    if(GetOpenedItem())
    {
        GetOpenedItem()->Draw(true);
    }
}


int Menu::DrawTitle()
{
    Rectangle(Display::WIDTH - 1, Page::Title::HEIGHT).Draw(0, 0, Color::FILL);
    Rectangle(Display::WIDTH - 3, Page::Title::HEIGHT - 2).Fill(1, 1, Color::BLUE_10);
    if(Menu::GetOpenedItem())
    {
        return Menu::GetOpenedItem()->FullPath().Draw(5, 5, Color::FILL);
    }

    return CURRENT_PAGE->FullPath().Draw(5, 5, Color::FILL);
}


void Menu::DrawPagesUGO(int x)
{
    x += 10;

    static const int delta = 12;

    int numSubPages = CURRENT_PAGE->NumSubPages();

    if (RegIsControlSubPages() && numSubPages > 1)
    {
        for (int i = 0; i < numSubPages; i++) 
        {
            Rectangle(delta, 9).Draw(x + 5 + i * delta, 5, Color::GRAY_25);
        }

        int numPage = CURRENT_PAGE->CurrentSubPage();

        Rectangle(delta, 9).Fill(x + 5 + numPage * delta, 5, Color::FILL);

        SU::Int2String(numPage + 1, false, 1).Draw(x + 9 + numPage * delta, 5, Color::BACK);
    }
}

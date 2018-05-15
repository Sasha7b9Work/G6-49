#include "Menu/MenuItems.h"
#include "Display/DisplayTypes.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Control::Draw(int x, int y, bool)
{
    if (type == Item_Choice)
    {
        Painter::DrawText(x + 5, y + 5, Title());
        Painter::DrawTextRelativelyRight(315, y + 30, ((Choice *)this)->NameCurrentSubItem());
    }
    else if (type == Item_Button)
    {
        Painter::DrawTextInRect(x + 5, y + 17, ITEM_WIDTH, Title());
    }
    else if (type == Item_ChoiceParameter)
    {
        Painter::DrawText(x + 5, y + 5, Title());
        Painter::DrawTextRelativelyRight(315, y + 30, ((ChoiceParameter *)this)->CurrentName());
    }
    else if (type == Item_SmallButton)
    {
        int size = 23;
        x += 20;
        y += 17;
        Painter::DrawRectangle(x, y, size, size);
        SButton *button = (SButton*)this;
        if (button->funcForDraw)
        {
            button->funcForDraw(x, y);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::DrawOpened()
{
    int width = ITEM_WIDTH;
    int height = NumSubItems() * 10 + 2 + 12;

    int y = MP_TITLE_HEIGHT + PositionOnPage() * MI_HEIGHT;
    int x = SCREEN_WIDTH - ITEM_WIDTH - 20;

    y += (MI_HEIGHT - height) / 2;

    ++y;

    Painter::FillRegion(x, y, width, height, Color::BACK);
    Painter::DrawRectangle(x, y, width, height, Color::FILL);
    Painter::DrawHLine(y + 12, x, x + width);

    Painter::DrawTextRelativelyRight(x + width - 2, y + 2, Title());

    y += 14;

    for (int i = 0; i < NumSubItems(); i++)
    {
        if (CurrentChoice() == i)
        {
            Painter::FillRegion(x + 2, y, ITEM_WIDTH - 4, 8, Color::FILL);
            Painter::DrawText(x + 3, y, NameSubItem(i), Color::BACK);
        }
        else
        {
            Painter::DrawText(x + 3, y, NameSubItem(i), Color::FILL);
        }
        y += 10;
    }
}

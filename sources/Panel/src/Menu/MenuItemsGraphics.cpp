#include "Menu/MenuItems.h"
#include "Display/DisplayTypes.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Control::Draw(int x, int y, bool)
{
    bool pressed = IsPressed();
    bool opened = IsOpened();

    if (type == Item_Choice)
    {
        Painter::FillRegion(x + 2, y + 2, ITEM_WIDTH - 5, 15, pressed || opened ? Color::GRAY_50 : Color::GREEN_10);
        Painter::DrawText(x + 5, y + 5, Title(), pressed || opened ? Color::BACK : Color::FILL);
        Painter::FillRegion(x + 2, y + 19, ITEM_WIDTH - 5, 34, Color::GREEN_25);
        Painter::DrawTextRelativelyRight(315, y + 30, ((Choice *)this)->NameCurrentSubItem(), Color::BACK);
    }
    else if (type == Item_Button)
    {
        Painter::FillRegion(x + 2, y + 2, ITEM_WIDTH - 5, MI_HEIGHT - 4, Color::GREEN_10);
        Painter::SetColor(Color::FILL);
        Painter::DrawTextInRect(x + 5, y + 17, ITEM_WIDTH, Title());
    }
    else if (type == Item_ChoiceParameter)
    {
        Painter::FillRegion(x + 2, y + 2, ITEM_WIDTH - 5, 15, Color::GREEN_10);
        Painter::DrawText(x + 5, y + 5, Title(), Color::FILL);
        Painter::FillRegion(x + 2, y + 19, ITEM_WIDTH - 5, 34, Color::GREEN_25);
        Painter::DrawTextRelativelyRight(315, y + 30, ((ChoiceParameter *)this)->CurrentName(), Color::BACK);
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
    else
    {
        Painter::FillRegion(x + 2, y + 2, ITEM_WIDTH - 5, MI_HEIGHT - 4, Color::GREEN_25);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::DrawOpened()
{
    int width = ITEM_WIDTH;
    int height = GetHeightOpened();

    int y = MP_TITLE_HEIGHT + PositionOnPage() * MI_HEIGHT;
    int x = SCREEN_WIDTH - ITEM_WIDTH - 20;

    y += (MI_HEIGHT - height) / 2;

    ++y;

    Painter::FillRegion(x, y, width, height, Color::GRAY_50);
    Painter::DrawRectangle(x, y, width, height, Color::FILL);
    Painter::DrawHLine(y + 12, x, x + width);
    Painter::DrawRectangle(x - 1, y - 1, width + 2, height + 2, Color::BACK);
    Painter::DrawTextRelativelyRight(x + width - 2, y + 2, Title());

    y += 14;

    for (int i = 0; i < NumSubItems(); i++)
    {
        if (CurrentChoice() == i)
        {
            Painter::FillRegion(x + 2, y, ITEM_WIDTH - 4, 8, Color::GREEN_50);
            Painter::DrawHLine(y - 1, x + 1, x - 1 + ITEM_WIDTH - 1, Color::GREEN_75);
            Painter::DrawVLine(x + 1, y - 1, y - 1 + 10);
            Painter::DrawHLine(y + 9, x + 1, x + ITEM_WIDTH - 1, Color::GREEN_25);
            Painter::DrawVLine(x - 1 + ITEM_WIDTH, y - 1, y + 9);
            Painter::DrawText(x + 3, y, NameSubItem(i), Color::BACK);
        }
        else
        {
            Painter::DrawText(x + 3, y, NameSubItem(i), Color::FILL);
        }
        y += 10;
    }
}

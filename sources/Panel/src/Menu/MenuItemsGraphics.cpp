#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Display/DisplayTypes.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Control::Draw(bool opened, int x, int y) const
{
    if (type == Control_Choice)
    {
        ((Choice *)this)->Draw(opened, x, y);
    }
    else if (type == Control_Button)
    {
        ((Button *)this)->Draw(x, y);
    }
    else if (type == Control_ChoiceParameter)
    {
        ((ChoiceParameter *)this)->Draw(opened, x, y);
    }
    else if (type == Control_SmallButton)
    {
        ((SButton *)this)->Draw(x, y);
    }
    else
    {
        Painter::FillRegion(x + 2, y + 2, ITEM_WIDTH - 5, MI_HEIGHT - 4, Menu::OpenedItem() ? Color::GRAY_10 : Color::GREEN_25);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void SButton::Draw(int x, int y)
{
    int size = 23;
    x += 20;
    y += 17;
    Painter::DrawRectangle(x, y, size, size);
    if (funcForDraw)
    {
        funcForDraw(x, y);
    }

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Button::Draw(int x, int y)
{
    bool isShade = IsShade();

    Painter::FillRegion(x + 2, y + 2, ITEM_WIDTH - 5, MI_HEIGHT - 4, isShade ? Color::GRAY_10 : Color::GREEN_10);
    Painter::SetColor(isShade ? Color::GRAY_25 : Color::FILL);
    Painter::DrawTextInRect(x + 5, y + 17, ITEM_WIDTH, Title());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void ChoiceParameter::Draw(bool opened, int x, int y)
{
    if(opened)
    {
    }
    else
    {
        bool pressed = IsPressed();
        bool isShade = IsShade();
        opened = IsOpened();

        Painter::FillRegion(x + 2, y + 2, ITEM_WIDTH - 5, 15, pressed || opened ? Color::GRAY_50 : (isShade ? Color::GRAY_10 : Color::GREEN_10));
        Painter::DrawText(x + 5, y + 5, Title(), pressed || opened ? Color::BACK : (isShade ? Color::GRAY_25 : Color::FILL));
        Painter::FillRegion(x + 2, y + 19, ITEM_WIDTH - 5, 34, isShade ? Color::GRAY_10 : Color::GREEN_25);
        Painter::DrawTextRelativelyRight(315, y + 30, NameCurrentSubItem(), Color::BACK);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::Draw(bool opened, int x, int y)
{
    Step();

    if(opened)
    {
        int width = ITEM_WIDTH;
        int height = GetHeightOpened();
        
        if(x == -1)
        {
            y = MP_TITLE_HEIGHT + PositionOnPage() * MI_HEIGHT;
            x = SCREEN_WIDTH - ITEM_WIDTH - 20;
        }

        Painter::FillRegion(x, y, width, MI_TITLE_HEIGHT, Color::GRAY_50);
        Painter::FillRegion(x, y + MI_TITLE_HEIGHT, width, height - MI_TITLE_HEIGHT, Color::BACK);
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
    else
    {
        bool pressed = IsPressed();
        bool isShade = IsShade();
        opened = IsOpened();

        Painter::FillRegion(x + 2, y + 2, ITEM_WIDTH - 5, 15, pressed || opened ? Color::GRAY_50 : (isShade ? Color::GRAY_10 : Color::GREEN_10));
        Painter::DrawText(x + 5, y + 5, Title(), pressed || opened? Color::BACK : (isShade ? Color::GRAY_25 : Color::FILL));
        Painter::FillRegion(x + 2, y + 19, ITEM_WIDTH - 5, 34, isShade ? Color::GRAY_10 : Color::GREEN_25);
        Painter::DrawTextRelativelyRight(315, y + 30, NameCurrentSubItem(), Color::BACK);
    }
}

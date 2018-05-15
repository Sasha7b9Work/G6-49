#include "Menu/MenuItems.h"
#include "Display/DisplayTypes.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawOpenedChoice(Choice *choice);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Control::Draw(int x, int y, bool opened)
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
        Painter::DrawTextRelativelyRight(315, y + 30, ChoiceWaveParameter_CurrentName((ChoiceParameter *)this));
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
void DrawOpenedItem()
{
    if (OPENED_ITEM)
    {
        TypeItem typeItem = ((Control *)OPENED_ITEM)->Type();

        if (typeItem == Item_Choice)
        {
            DrawOpenedChoice((Choice *)OPENED_ITEM);
        }
        else if (typeItem == Item_ChoiceParameter)
        {
            DrawOpenedChoice((Choice *)OPENED_ITEM);
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawOpenedChoice(Choice *choice)
{
    int width = ITEM_WIDTH;
    int height = choice->NumSubItems() * 10 + 2 + 12;

    int y = MP_TITLE_HEIGHT + PositionOnPage(choice) * MI_HEIGHT;
    int x = SCREEN_WIDTH - ITEM_WIDTH - 20;

    y += (MI_HEIGHT - height) / 2;

    ++y;

    Painter::FillRegion(x, y, width, height, Color::BACK);
    Painter::DrawRectangle(x, y, width, height, Color::FILL);
    Painter::DrawHLine(y + 12, x, x + width);

    Painter::DrawTextRelativelyRight(x + width - 2, y + 2, choice->Title());

    y += 14;

    for (int i = 0; i < choice->NumSubItems(); i++)
    {
        if (Choice_CurrentChoice(choice) == i)
        {
            Painter::FillRegion(x + 2, y, ITEM_WIDTH - 4, 8, Color::FILL);
            Painter::DrawText(x + 3, y, Choice_Name(choice, i), Color::BACK);
        }
        else
        {
            Painter::DrawText(x + 3, y, Choice_Name(choice, i), Color::FILL);
        }
        y += 10;
    }
}
